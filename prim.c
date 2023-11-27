#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INSTRUCTIONS 10000
#define INFINITY 2147483648 // 2147483647+1

// structures ================================================================
typedef struct EDGE{
    int src;
    int dest;
    int weight;
    int colour; // 0 = grey, 1 = blue, 2 = red
}Edge;

typedef struct SET{
    int data;
    int rank;
    struct SET* parent;
}Set;

typedef struct GRAPH{
    int num_nodes;
    int num_edges;
    int **edge_list;
} Graph;

typedef struct INSTR{
    char instruction;
    Edge edge;
}Instr;



// set functions ===========================================================
void add_set(){

}

void union_set(){

}

void find_set(){

}

void make_set(int node_name){

}

// sorting functions ==========================================================
void swap(Edge* edge1, Edge* edge2){
    Edge temp = *edge1;
    *edge1 = *edge2;
    *edge2 = temp;
}

int partition(Edge* edge_list, int left, int right){
    int pivot = edge_list[right].weight;
    int i = left -1;
    for(int j = left; j <= right; j++){
        if(edge_list[j].weight <= pivot){
            i++;
            swap(&edge_list[i], &edge_list[j]);
        }
    }
    swap(&edge_list[i + 1], &edge_list[right]);
    return i + 1;
}

void quicksort_edges(Edge* edge_list, int left, int right){
    if(left < right){
        int pivot = partition(edge_list, left, right);
        quicksort_edges(edge_list, left, pivot - 1);
        quicksort_edges(edge_list, pivot, right);
    }
}

// functions =================================================================
void prim(Graph* graph){
    

}


// edge functions ===========================================================
void insertEdge(Edge new_edge, Graph* graph){
    new_edge.colour = 0; // set edge colour to 0

    if(graph->edge_list[new_edge.src-1][new_edge.dest-1] > 0){
        return;
    }else{
        graph->edge_list[new_edge.src-1][new_edge.dest-1] = new_edge.weight;
        graph->edge_list[new_edge.dest-1][new_edge.src-1] = new_edge.weight;
        graph->num_edges++;
    }
    printf("inserted edge: %d %d %d\n", new_edge.src, new_edge.dest, new_edge.weight);
}

void changeWeight(Edge edge, Graph* graph){
    if(graph->edge_list[edge.src-1][edge.dest-1] > 0){
        graph->edge_list[edge.src-1][edge.dest-1] = edge.weight;
        graph->edge_list[edge.dest-1][edge.src-1] = edge.weight;
    }

    //if it doesn't exist, don't do anything
    printf("changed weight: %d %d %d\n", edge.src, edge.dest, edge.weight);
}

void deleteEdge(Edge edge, Graph* graph){
    if(graph->edge_list[edge.src-1][edge.dest-1] > 0){
        graph->edge_list[edge.src-1][edge.dest-1] = -1;
        graph->edge_list[edge.dest-1][edge.src-1] = -1;
        graph->num_edges--;
    }
    //if it doesn't exist, don't do anything
}

void findMST(Graph* graph, FILE* output_file){
    prim(graph);
    //print total weight
    int total_weight = 0;
    fprintf(output_file, "%d\n", total_weight);
}


// main ======================================================================
int main(){

    FILE *input_file = fopen("mst.in", "r");
    FILE *output_file = fopen("mst.out", "w");

    //scan the number of nodes
    int num_nodes = 0;
    fscanf(input_file, "%d", &num_nodes);

    Instr instr_list[MAX_INSTRUCTIONS];
    
    //create new graph
    Graph graph;
    graph.num_nodes = num_nodes;
    graph.num_edges = 0;
    graph.edge_list = malloc(sizeof(int) * (num_nodes));
    for(int i = 0; i < num_nodes; i++){
        graph.edge_list[i] = malloc(sizeof(int) * (num_nodes));
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
            fscanf(input_file, "%d", &instruction.edge.src);
            fscanf(input_file, "%d", &instruction.edge.dest);
            //store smaller node to src
            if(instruction.edge.src > instruction.edge.dest){
                int temp = instruction.edge.src;
                instruction.edge.src = instruction.edge.dest;
                instruction.edge.dest = temp;
            }
            instr_list[list_index++] = instruction;
        } //changeWeight & insertEdge instruction
        else{
            Instr instruction;
            instruction.instruction = instr_name[0];
            fscanf(input_file, "%d", &instruction.edge.src);
            fscanf(input_file, "%d", &instruction.edge.dest);
            fscanf(input_file, "%d", &instruction.edge.weight);

            //store smaller node to src
            if(instruction.edge.src > instruction.edge.dest){
                int temp = instruction.edge.src;
                instruction.edge.src = instruction.edge.dest;
                instruction.edge.dest = temp;
            }

            instr_list[list_index++] = instruction;
        }
    }

    printf("num_edges: %d\n", graph.num_edges);

    fclose(input_file);
    fclose(output_file);
}