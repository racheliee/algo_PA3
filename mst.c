#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INSTRUCTIONS 10000


// structures ================================================================
typedef struct EDGE{
    int src_node;
    int dest_node;
    int weight;
}Edge;

typedef struct NODE{
    int data;
    int rank;
    int colour; // 0 = grey, 1 = blue, 2 = red
    struct NODE* parent;
}Node;

typedef struct TREE{
    int num_nodes;
    int num_edges;
    Edge* edge_list;
} Tree;

typedef struct INSTR{
    char instruction[20];
    Edge edge;
}Instr;



// set functions ===========================================================

void add_set(){

}

void union_set(){

}

void find_set(){

}

void make_set(){

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
void kruskal_algo(Edge *edge_list, int num_edges){
    quicksort_edges(edge_list, 0, num_edges - 1);


}




// main ======================================================================
int main(){

    FILE *input_file = fopen("mst.in", "r");
    FILE *output_file = fopen("mst.out", "w");

    //scan the number of nodes
    int num_nodes = 0;
    fscanf(input_file, "%d", &num_nodes);

    Instr instr_list[MAX_INSTRUCTIONS];
    char instr_name[20];
    int list_index = 0;
    int start_index = 0;
    while(fscanf(input_file, "%s", instr_name) == 1){
        //findMST instruction
        if(strcmp(instr_name, "findMST") == 0){
            for(int i = start_index; i < list_index; i++){
                if(strcmp(instr_list[i].instruction, "insertEdge") == 0){
                    
                }
                else if(strcmp(instr_list[i].instruction, "changeWeight") == 0){
                    
                }
                else if(strcmp(instr_list[i].instruction, "deleteEdge") == 0){
                    
                }
            }
            //findMST
            //print total weight
            
            start_index = list_index;
        } //deleteEdge instruction
        else if (strcmp(instr_name, "deleteEdge") == 0){
            Instr instruction;
            instruction.edge.weight = 0;
            strcpy(instruction.instruction, instr_name);
            fscanf(input_file, "%d", &instruction.edge.src_node);
            fscanf(input_file, "%d", &instruction.edge.src_node);
            instr_list[list_index++] = instruction;
        } //changeWeight & insertEdge instruction
        else{
            Instr instruction;
            strcpy(instruction.instruction, instr_name);
            fscanf(input_file, "%d", &instruction.edge.src_node);
            fscanf(input_file, "%d", &instruction.edge.dest_node);
            fscanf(input_file, "%d", &instruction.edge.weight);
            instr_list[list_index++] = instruction;
        }
    }




}