#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_INSTRUCTIONS 10000
#define INT_MAX 2147483647
#define INPUT_FILE "mst.in"
#define OUTPUT_FILE "mst.out"

// structures ================================================================
typedef struct EDGE{
    int dest;
    int weight;
    struct EDGE* next;
}Edge;

typedef struct ADJLIST{
    Edge* head;
} AdjList;

typedef struct GRAPH{
    int num_nodes;
    int num_edges;
    AdjList* adj_list;
    int **adj_matrix; //linked list로 연결하는 형태로 바꾸기
} Graph;

typedef struct VERTEX{
    int name;
    int sort_key;
    int key;
    int inMST; //checks if vertex is in MST
    int inQueue; //checks if vertex is in queue
    struct VERTEX* edge_parent;
    struct VERTEX* left;
    struct VERTEX* right;
    struct VERTEX* parent;
    struct VERTEX* child;
    int degree;
    int mark;
} Vertex;

typedef struct VERTEX_PTR{
    Vertex* vertex;
} Vertex_ptr;

//create a fibonacci heap
typedef struct HEAP{
    int size;
    int degree;
    int* pos; //stores position of vertex in heap
    Vertex* min; //stores the minimum vertex
    Vertex **vertex;
}Heap;

typedef struct INSTR{
    char instruction;
    int node1;
    int node2;
    int weight;
}Instr;

// heap functions ===========================================================
void swap_int(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swap_vertex(Vertex** a, Vertex** b){
    Vertex* temp = *a;
    *a = *b;
    *b = temp;
}

void link(Heap* heap, Vertex* node1, Vertex* node2){
    node1->left->right = node1->right;
    node1->right->left = node1->left;

    if(node1->right == node1){
        heap->min = node1;
    }

    node2->left = node2;
    node2->right = node2;
    node2->parent = node1;

    if(node1->child == NULL){
        node1->child = node2;
    }
    node2->right = node1->child;
    node2->left = node1->child->left;
    node1->child->left->right = node2;
    node1->child->left = node2;
    
    if(node2->sort_key < node1->sort_key){
        node1->child = node2;
    }

    node1->degree++;
}

int get_degree(int size){
    int degree = 0;
    while(size > 0){
        size = size >> 1;
        degree++;
    }
    return degree;
}


void consolidate(Heap* heap){
    int degree = get_degree(heap->size);
    Vertex* arr[degree];
    for(int i = 0; i < degree; i++){
        arr[i] = NULL;
    }
    int t;
    Vertex* v1, *v2, *v3;
    v1 = heap->min;

    do{
        t = v1->degree;
        while(arr[t] != NULL){
            v2 = arr[t];
            if(v1->sort_key > v2->sort_key){
                swap_vertex(&v1, &v2);
            }
            if(v2 == heap->min){
                heap->min = v1;
            }
            link(heap, v2, v1);
            if(v1->right == v1){
                heap->min = v1;
            }
            arr[t] = NULL;
            t++;
        }
        arr[t] = v1;
        v1 = v1->right;
    }while(v1 != heap->min);

    heap->min = NULL;
    for(int i = 0; i < degree; i++){
        if(arr[i] != NULL){
            arr[i]->left = arr[i];
            arr[i]->right = arr[i];
            if(heap->min == NULL){
                heap->min = arr[i];
            }else{
                heap->min->left->right = arr[i];
                arr[i]->right = heap->min;
                arr[i]->left = heap->min->left;
                heap->min->left = arr[i];
                if(arr[i]->sort_key < heap->min->sort_key){
                    heap->min = arr[i];
                }
            }
            if(heap->min == NULL){
                heap->min = arr[i];
            }
            else if(arr[i]->sort_key < heap->min->sort_key){
                heap->min = arr[i];
            }
        }
    }  
}

//get the minimum vertex from heap
Vertex* extractMin(Heap* heap){
    if(heap->size == 0){
        return NULL;
    }

    Vertex* min_vertex = heap->min;
    min_vertex->sort_key = INT_MAX;

    Vertex* temp = min_vertex;
    Vertex* look = temp;
    Vertex* child = NULL;

    if(temp->child != NULL){
        child = temp->child;
        do{
            look = child->right;
            heap->min->left->right = child;
            child->right = heap->min;
            child->left = heap->min->left;
            heap->min->left = child;
            if(child->sort_key < heap->min->sort_key){
                heap->min = child;
            }
            child->parent = NULL;
            child = look;
        }while(look != temp->child);
    }

    temp->left->right = temp->right;
    temp->right->left = temp->left;
    heap->min = temp->right;

    if(temp == temp->right && temp->child == NULL){
        heap->min = NULL;
    }
    else{
        heap->min = temp->right;
        //heap->pos[temp->name] = 0;
        consolidate(heap);
    }

    heap->size--;

    return min_vertex;
}

void cut(Heap* heap, Vertex* node){
    if(node->right == node){
        node->parent->child = NULL;
    }
    else{
        node->right->left = node->left;
        node->left->right = node->right;
        if(node->parent->child == node){
            node->parent->child = node->right;
        }
    }
    node->parent->degree--;
    node->left = node;
    node->right = node;
    heap->min->left->right = node;
    node->right = heap->min;
    node->left = heap->min->left;
    heap->min->left = node;
    // node->left = heap->min;
    // node->right = heap->min->right;
    // heap->min->right = node;
    // node->right->left = node;
    node->parent = NULL;
    node->mark = 0;
}

void cascading_cut(Heap* heap, Vertex* parent_node){
    Vertex* temp = parent_node->parent;
    if(temp != NULL){
        if(parent_node->mark == 0){
            parent_node->mark = 1;
        }
        else{
            cut(heap, parent_node);
            cascading_cut(heap, temp);
        }
    }
}

void initHeap(Heap* heap, int num_nodes){
    heap->size = num_nodes;
    heap->min = NULL;
    heap->degree = 0;
    heap->pos = malloc(sizeof(int) * num_nodes);
    heap->vertex = malloc(sizeof(Vertex*) * num_nodes);

    //initialize all vertices; key values are all set to infinity
    for(int i = 0; i < num_nodes; i++){
        heap->vertex[i] = malloc(sizeof(Vertex));
        heap->vertex[i]->name = i;
        heap->vertex[i]->sort_key = INT_MAX;
        heap->vertex[i]->key = INT_MAX;
        heap->vertex[i]->inMST = 0;
        heap->vertex[i]->inQueue = 1;
        heap->vertex[i]->edge_parent = NULL;
        heap->vertex[i]->left = NULL;
        heap->vertex[i]->right = NULL;
        heap->vertex[i]->parent = NULL;
        heap->vertex[i]->child = NULL;
        heap->vertex[i]->degree = 0;
        heap->vertex[i]->mark = 0;
        heap->pos[i] = i;
    }
}


// functions =================================================================
int prim(Graph* graph){
    if(graph->num_edges == 0){
        return -1;
    }

    Heap queue;
    initHeap(&queue, graph->num_nodes);

    Vertex_ptr* vertex_ptrs = malloc(sizeof(Vertex_ptr) * graph->num_nodes);
    for(int i = 0; i < graph->num_nodes; i++){
        vertex_ptrs[i].vertex = queue.vertex[i];
    }

    queue.vertex[0]->sort_key = 0;
    queue.vertex[0]->key = 0;
    queue.vertex[0]->edge_parent = NULL; //first element doesn't have a parent
    queue.vertex[0]->inMST = 1;

    int total_weight = 0;

    while(queue.size != 0){
        Vertex* min_node = extractMin(&queue);
        int u = min_node->name;
        min_node->inMST = 1;
        total_weight += min_node->key;
        min_node->inQueue = 0;

        Edge* adjlist = graph->adj_list[u].head;
        while(adjlist != NULL){
            if(queue.vertex[vertex_ptrs->vertex->name]->inMST == 0 && queue.vertex[vertex_ptrs->vertex->name]->inQueue == 1 && adjlist->weight < vertex_ptrs[adjlist->dest].vertex->key){
                vertex_ptrs[adjlist->dest].vertex->edge_parent = min_node;
                vertex_ptrs[adjlist->dest].vertex->sort_key = adjlist->weight;
                vertex_ptrs[adjlist->dest].vertex->key = adjlist->weight;    

                cut(&queue, vertex_ptrs[adjlist->dest].vertex);
                cascading_cut(&queue, vertex_ptrs[adjlist->dest].vertex->parent);
            }
            adjlist = adjlist->next;
        }
    }

    //check if all vertices have parents except for the first one
    for(int i = 0; i < graph->num_nodes; i++){
        if(queue.vertex[i]->edge_parent == NULL && queue.vertex[i]->name != 0){
            return -1;
        }
    }
   
    return total_weight;
}

// input instruction functions ===========================================================
void insertEdge(Instr instr, Graph* graph){
    if(graph->adj_matrix[instr.node1-1][instr.node2-1] == 0){
        graph->adj_matrix[instr.node1-1][instr.node2-1] = instr.weight;
        graph->adj_matrix[instr.node2-1][instr.node1-1] = instr.weight;
        graph->num_edges++;

        //insert into adj_list
        Edge* new_edge = malloc(sizeof(Edge));
        new_edge->dest = instr.node2-1;
        new_edge->weight = instr.weight;
        new_edge->next = graph->adj_list[instr.node1-1].head;
        graph->adj_list[instr.node1-1].head = new_edge;

        //insert into adj_list on the other side
        Edge* new_edge2 = malloc(sizeof(Edge));
        new_edge2->dest = instr.node1-1;
        new_edge2->weight = instr.weight;
        new_edge2->next = graph->adj_list[instr.node2-1].head;
        graph->adj_list[instr.node2-1].head = new_edge2;
    }
}

void changeWeight(Instr instr, Graph* graph){
    if(graph->adj_matrix[instr.node1-1][instr.node2-1] > 0){
        graph->adj_matrix[instr.node1-1][instr.node2-1] = instr.weight;
        graph->adj_matrix[instr.node2-1][instr.node1-1] = instr.weight;

        //change weight in adj_list
        Edge* temp = graph->adj_list[instr.node1-1].head;
        while(temp != NULL){
            if(temp->dest == instr.node2-1){
                temp->weight = instr.weight;
                break;
            }
            temp = temp->next;
        }

        //change weight in adj_list on the other side
        temp = graph->adj_list[instr.node2-1].head;
        while(temp != NULL){
            if(temp->dest == instr.node1-1){
                temp->weight = instr.weight;
                break;
            }
            temp = temp->next;
        }
    }
}

void deleteEdge(Instr instr , Graph* graph){
    if(graph->adj_matrix[instr.node1-1][instr.node2-1] > 0){
        graph->adj_matrix[instr.node1-1][instr.node2-1] = 0;
        graph->adj_matrix[instr.node2-1][instr.node1-1] = 0;
        graph->num_edges--;

        //delete from adj_list
        Edge* temp = graph->adj_list[instr.node1-1].head;
        Edge* prev = NULL;
        while(temp != NULL){
            if(temp->dest == instr.node2-1){
                if(prev == NULL){
                    graph->adj_list[instr.node1-1].head = temp->next;
                }
                else{
                    prev->next = temp->next;
                }
                free(temp);
                break;
            }
            prev = temp;
            temp = temp->next;
        }

        //delete from adj_list on the other side
        temp = graph->adj_list[instr.node2-1].head;
        prev = NULL;
        while(temp != NULL){
            if(temp->dest == instr.node1-1){
                if(prev == NULL){
                    graph->adj_list[instr.node2-1].head = temp->next;
                }
                else{
                    prev->next = temp->next;
                }
                free(temp);
                break;
            }
            prev = temp;
            temp = temp->next;
        }
    }
}

void findMST(Graph* graph, FILE* output_file){
    int mst_weight = prim(graph);
    
    //print total weight
    if(mst_weight == -1){
        // printf("Disconnected\n");
        fprintf(output_file, "Disconnected\n");
    }
    else {
        // printf("%d\n", mst_weight);
        fprintf(output_file, "%d\n", mst_weight);
    }
}


// main ======================================================================
int main(){
    //at beginning of main
    float TIME = 0;
    clock_t start = clock();

    FILE *input_file = fopen(INPUT_FILE, "r");
    FILE *output_file = fopen(OUTPUT_FILE, "w");

    //scan the number of nodes
    int num_nodes = 0;
    fscanf(input_file, "%d", &num_nodes);

    Instr instr_list[MAX_INSTRUCTIONS];
    
    //create new graph
    Graph graph;
    graph.num_nodes = num_nodes;
    graph.num_edges = 0;
    graph.adj_matrix = malloc(sizeof(int*) * (num_nodes));
    graph.adj_list = malloc(sizeof(AdjList) * num_nodes);
    for(int i = 0; i < num_nodes; i++){
        graph.adj_list[i].head = NULL;
        graph.adj_matrix[i] = malloc(sizeof(int) * (num_nodes));
        for(int j = 0; j < num_nodes; j++){
            graph.adj_matrix[i][j] = 0;
        }
    }

    //scan the instructions
    char instr_name[20];
    int list_index = 0;
    int start_index = 0;
    while(fscanf(input_file, "%s", instr_name) == 1){
        //findMST instruction
        if(instr_name[0] == 'f'){
            for(int i = start_index; i < list_index; i++){
                //insertEdge
                if(instr_list[i].instruction == 'i'){
                    insertEdge(instr_list[i], &graph);
                }
                //changeWeight
                else if(instr_list[i].instruction == 'c'){
                    changeWeight(instr_list[i], &graph);
                }
                //deleteEdge
                else if(instr_list[i].instruction == 'd'){
                    deleteEdge(instr_list[i], &graph);
                }
            }
            //findMST
            findMST(&graph, output_file);
            start_index = list_index;
        } //deleteEdge instruction
        else if (instr_name[0] == 'd'){
            Instr instruction;
            instruction.instruction = instr_name[0];
            fscanf(input_file, "%d %d", &instruction.node1, &instruction.node2);
            instr_list[list_index++] = instruction;
        } //changeWeight & insertEdge instruction
        else{
            Instr instruction;
            instruction.instruction = instr_name[0];
            fscanf(input_file, "%d %d %d", &instruction.node1, &instruction.node2, &instruction.weight);
            instr_list[list_index++] = instruction;
        }
    }

    fclose(input_file);
    fclose(output_file);

    //at end of main
    TIME += ((int)clock() - start) / (CLOCKS_PER_SEC / 1000);
    printf("TIME : %f ms\n", TIME);
}