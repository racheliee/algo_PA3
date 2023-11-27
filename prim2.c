#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INSTRUCTIONS 10000
#define INFINITY 21474836487

// structures ================================================================
typedef struct EDGE{
    int src;
    int dest;
    int weight;
    int colour; // 0 = grey, 1 = blue, 2 = red
}Edge;

typedef struct GRAPH{
    int num_nodes;
    int num_edges;
    int **adj_matrix;
} Graph;

typedef struct VERTEX{
    int name;
    int key;
} Vertex;

typedef struct HEAP{
    int size;
    Vertex **vertex;
}Heap;

typedef struct INSTR{
    char instruction;
    Edge edge;
}Instr;

// heap functions ===========================================================
void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(Heap* heap, int index){

}

//get the minimum vertex from heap
Vertex* extractMin(Heap* heap){

}

Heap* buildHeap(int num_nodes){

}

// functions =================================================================
int prim(Graph* graph){
   
    return 0;
}


// input instruction functions ===========================================================
void insertEdge(Edge new_edge, Graph* graph){
    if(graph->adj_matrix[new_edge.src-1][new_edge.dest-1] == 0){
        graph->adj_matrix[new_edge.src-1][new_edge.dest-1] = new_edge.weight;
        graph->adj_matrix[new_edge.dest-1][new_edge.src-1] = new_edge.weight;
        graph->num_edges++;
    }
    printf("inserted edge: %d %d %d\n", new_edge.src, new_edge.dest, new_edge.weight);
}

void changeWeight(Edge edge, Graph* graph){
    if(graph->adj_matrix[edge.src-1][edge.dest-1] > 0){
        graph->adj_matrix[edge.src-1][edge.dest-1] = edge.weight;
        graph->adj_matrix[edge.dest-1][edge.src-1] = edge.weight;
    }

    //if it doesn't exist, don't do anything
    printf("changed weight: %d %d %d\n", edge.src, edge.dest, edge.weight);
}

void deleteEdge(Edge edge, Graph* graph){
    if(graph->adj_matrix[edge.src-1][edge.dest-1] > 0){
        graph->adj_matrix[edge.src-1][edge.dest-1] = 0;
        graph->adj_matrix[edge.dest-1][edge.src-1] = 0;
        graph->num_edges--;
    }
    //if it doesn't exist, don't do anything
    printf("deleted edge: %d %d\n", edge.src, edge.dest);
}

void findMST(Graph* graph, FILE* output_file){
    int mst_weight = prim(graph);

    //print total weight
    if(mst_weight == -1)
        fprintf(output_file, "Disconnected\n");
    else 
        fprintf(output_file, "%d\n", mst_weight);
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
    graph.adj_matrix = malloc(sizeof(int) * (num_nodes));
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
            fscanf(input_file, "%d %d %d", &instruction.edge.src, &instruction.edge.dest, &instruction.edge.weight);

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