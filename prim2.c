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
    int src;
    int dest;
    int weight;
}Edge;

typedef struct GRAPH{
    int num_nodes;
    int num_edges;
    int **adj_matrix; //linked list로 연결하는 형태로 바꾸기
} Graph;

typedef struct VERTEX{
    int name;
    int sort_key;
    int key;
    int inMST; //checks if vertex is in MST
    struct VERTEX* parent;
} Vertex;

typedef struct HEAP{
    int size;
    int* pos; //stores position of vertex in heap
    Vertex **vertex;
}Heap;

typedef struct INSTR{
    char instruction;
    Edge edge;
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

void heapifyDown(Heap* heap, int index){
    int left = index * 2+1; // left child index
    int right = index * 2+2; //right child index
    int min = index;

    if(left < heap->size && heap->vertex[left]->sort_key < heap->vertex[min]->sort_key){
        min = left;
    }
    if(right < heap->size && heap->vertex[right]->sort_key < heap->vertex[min]->sort_key){
        min = right;
    }

    if(min != index){
        swap_vertex(&heap->vertex[index], &heap->vertex[min]);
        swap_int(&heap->pos[heap->vertex[index]->name], &heap->pos[heap->vertex[min]->name]);

        heapifyDown(heap, min);
    }
}

void heapifyUp(Heap* heap, int index){
    while(index > 0 && heap->vertex[(index-1)/2]->sort_key > heap->vertex[index]->sort_key){
        swap_vertex(&heap->vertex[index], &heap->vertex[(index-1)/2]);
        swap_int(&heap->pos[heap->vertex[index]->name], &heap->pos[heap->vertex[(index-1)/2]->name]);

        index = (index-1)/2;
    }
}

void changeKey(Heap* heap, int vertex_name, int key){
    int index = heap->pos[vertex_name];
    
    heap->vertex[index]->sort_key = key;
    printf("changeKey of node %d to %d\n", vertex_name, key);

    heapifyUp(heap, index); //reorder heap
}


//get the minimum vertex from heap
Vertex* extractMin(Heap* heap){
    if(heap->size == 0){
        return NULL;
    }

    Vertex* min = heap->vertex[0];
    min->sort_key = INT_MAX;

    // Vertex* last = heap->vertex[heap->size-1];
    // heap->vertex[0] = last;

    // heap->pos[min->name] = heap->size-1;
    // heap->pos[last->name] = 0;

    swap_vertex(&heap->vertex[0], &heap->vertex[heap->size-1]);
    swap_int(&heap->pos[heap->vertex[0]->name], &heap->pos[heap->vertex[heap->size-1]->name]);

    heap->size--;
    heapifyDown(heap, 0);

    return min;
}

void initHeap(int num_nodes, Heap* heap){
    heap->size = num_nodes;
    heap->pos = malloc(sizeof(int) * num_nodes);
    heap->vertex = malloc(sizeof(Vertex*) * num_nodes);
}

// functions =================================================================
int prim(Graph* graph){
    if(graph->num_edges == 0){
        return -1;
    }

    Heap queue;
    initHeap(graph->num_nodes, &queue);

    //initialize all vertices; key values are all set to infinity
    for(int i = 0; i < graph->num_nodes; i++){
        queue.vertex[i] = malloc(sizeof(Vertex));
        queue.vertex[i]->name = i;
        queue.vertex[i]->sort_key = INT_MAX;
        queue.vertex[i]->key = INT_MAX;
        queue.vertex[i]->inMST = 0;
        queue.vertex[i]->parent = NULL;
        queue.pos[i] = i;
    }

    queue.vertex[0]->sort_key = 0;
    queue.vertex[0]->key = 0;
    queue.vertex[0]->parent = NULL; //first element doesn't have a parent
    queue.vertex[0]->inMST = 1;

    int total_weight = 0;

    while(queue.size != 0){

        // for(int i = 0; i < graph->num_nodes; i++){
        //             printf("%d ", queue.pos[i]);
        //         }
        //         printf("\n");
        // printf("sort keys: ");
        // for(int i = 0; i < graph->num_nodes; i++){
        //     printf("%d ", queue.vertex[i]->sort_key);
        // }
        // printf("\n");

        // printf("keys: ");
        // for(int i = 0; i < graph->num_nodes; i++){
        //     printf("%d ", queue.vertex[i]->key);
        // }
        // printf("\n");

        Vertex* min_node = extractMin(&queue);
        int u = min_node->name;
        min_node->inMST = 1;
        total_weight += min_node->key;
        // printf("min_node: %d\n", u);
        // printf("min_node key: %d\n", min_node->key);
        // printf("total_weight: %d\n", total_weight);
        // for(int i = 0; i < graph->num_nodes; i++){
        //             printf("%d ", queue.pos[i]);
        //         }
        //         printf("\n");

        // printf("sort keys: ");
        // for(int i = 0; i < graph->num_nodes; i++){
        //     printf("%d ", queue.vertex[i]->sort_key);
        // }
        // printf("\n");

        // printf("keys: ");
        // for(int i = 0; i < graph->num_nodes; i++){
        //     printf("%d ", queue.vertex[i]->key);
        // }
        // printf("\n");

        // printf("parents: ");
        // for(int i = 0; i < graph->num_nodes; i++){
        //     if(queue.vertex[i]->parent != NULL){
        //         printf("%d ", queue.vertex[i]->parent->name);
        //     }
        //     else{
        //         printf("NULL ");
        //     }
        // }
        // printf("\n");

        // printf("inMST: ");
        // for(int i = 0; i < graph->num_nodes; i++){
        //     printf("%d ", queue.vertex[i]->inMST);
        // }
        // printf("\n");

        for(int i = 0; i < graph->num_nodes; i++){

            if(queue.vertex[queue.pos[i]]->inMST == 0 && queue.pos[i] < queue.size && graph->adj_matrix[u][i] > 0 && graph->adj_matrix[u][i] < queue.vertex[queue.pos[i]]->key){
                // printf("adj: %d, key: %d\n", graph->adj_matrix[u][i], queue.vertex[i]->key);
                //printf("i: %d, size: %d\n", i, queue.size);
                //printf("i: %d, pos[i]: %d, size: %d\n", i, queue.pos[i], queue.size);
                queue.vertex[queue.pos[i]]->parent = min_node;
                // printf("parent of node %d is %d\n", queue.vertex[queue.pos[i]]->name, min_node->name);
                queue.vertex[queue.pos[i]]->sort_key = graph->adj_matrix[u][i];
                queue.vertex[queue.pos[i]]->key = graph->adj_matrix[u][i];
                // printf("changeKey of node %d to %d\n", queue.vertex[queue.pos[i]]->name, graph->adj_matrix[u][i]);
                heapifyUp(&queue, queue.pos[i]);

                // for(int i = 0; i < graph->num_nodes; i++){
                //     printf("%d ", queue.pos[i]);
                // }
                // printf("\n");
            }
        }
        // printf("\n");
    }

    //check if all vertices have parents except for the first one
    for(int i = 0; i < graph->num_nodes; i++){
        if(queue.vertex[i]->parent == NULL && queue.vertex[i]->name != 0){
            return -1;
        }
    }

    // for(int i = 0; i < graph->num_nodes; i++){
    //     if(queue.vertex[i]->parent != NULL){
    //         printf("%d -> %d weight: %d\n", queue.vertex[i]->name, queue.vertex[i]->parent->name, graph->adj_matrix[queue.vertex[i]->name][queue.vertex[i]->parent->name]);
    //         total_weight += graph->adj_matrix[queue.vertex[i]->name][queue.vertex[i]->parent->name];
    //     }
    // }
   
    return total_weight;
}

// input instruction functions ===========================================================
void insertEdge(Edge new_edge, Graph* graph){
    if(graph->adj_matrix[new_edge.src-1][new_edge.dest-1] == 0){
        graph->adj_matrix[new_edge.src-1][new_edge.dest-1] = new_edge.weight;
        graph->adj_matrix[new_edge.dest-1][new_edge.src-1] = new_edge.weight;
        graph->num_edges++;
    }
    // printf("inserted edge: %d %d %d\n", new_edge.src, new_edge.dest, new_edge.weight);
    // for(int i = 0; i < graph->num_nodes; i++){
    //     for(int j = 0; j < graph->num_nodes; j++){
    //         printf("%d ", graph->adj_matrix[i][j]);
    //     }
    //     printf("\n");
    // }
}

void changeWeight(Edge edge, Graph* graph){
    if(graph->adj_matrix[edge.src-1][edge.dest-1] > 0){
        graph->adj_matrix[edge.src-1][edge.dest-1] = edge.weight;
        graph->adj_matrix[edge.dest-1][edge.src-1] = edge.weight;
    }

    //if it doesn't exist, don't do anything
    // printf("changed weight: %d %d %d\n", edge.src, edge.dest, edge.weight);
    // for(int i = 0; i < graph->num_nodes; i++){
    //     for(int j = 0; j < graph->num_nodes; j++){
    //         printf("%d ", graph->adj_matrix[i][j]);
    //     }
    //     printf("\n");
    // }
}

void deleteEdge(Edge edge, Graph* graph){
    if(graph->adj_matrix[edge.src-1][edge.dest-1] > 0){
        graph->adj_matrix[edge.src-1][edge.dest-1] = 0;
        graph->adj_matrix[edge.dest-1][edge.src-1] = 0;
        graph->num_edges--;
    }
    //if it doesn't exist, don't do anything
    // printf("deleted edge: %d %d\n", edge.src, edge.dest);
    // for(int i = 0; i < graph->num_nodes; i++){
    //     for(int j = 0; j < graph->num_nodes; j++){
    //         printf("%d ", graph->adj_matrix[i][j]);
    //     }
    //     printf("\n");
    // }
}

void findMST(Graph* graph, FILE* output_file){
    int mst_weight = prim(graph);
    // printf("findMST\n");

    // for(int i = 0; i < graph->num_nodes; i++){
    //     for(int j = 0; j < graph->num_nodes; j++){
    //         printf("%d ", graph->adj_matrix[i][j]);
    //     }
    //     printf("\n");
    // }

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
    for(int i = 0; i < num_nodes; i++){
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
                    insertEdge(instr_list[i].edge, &graph);
                }
                //changeWeight
                else if(instr_list[i].instruction == 'c'){
                    changeWeight(instr_list[i].edge, &graph);
                }
                //deleteEdge
                else if(instr_list[i].instruction == 'd'){
                    deleteEdge(instr_list[i].edge, &graph);
                }
            }
            //findMST
            findMST(&graph, output_file);
            start_index = list_index;
        } //deleteEdge instruction
        else if (instr_name[0] == 'd'){
            Instr instruction;
            instruction.instruction = instr_name[0];
            fscanf(input_file, "%d %d", &instruction.edge.src, &instruction.edge.dest);
            instr_list[list_index++] = instruction;
        } //changeWeight & insertEdge instruction
        else{
            Instr instruction;
            instruction.instruction = instr_name[0];
            fscanf(input_file, "%d %d %d", &instruction.edge.src, &instruction.edge.dest, &instruction.edge.weight);
            instr_list[list_index++] = instruction;
        }
    }

    // printf("num_edges: %d\n", graph.num_edges);

    fclose(input_file);
    fclose(output_file);

    //at end of main
    TIME += ((int)clock() - start) / (CLOCKS_PER_SEC / 1000);
    printf("TIME : %f ms\n", TIME);
}