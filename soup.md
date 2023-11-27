# Objective
    - Design and implement an algorithm to dynamically maintain the minimum spanning tree (MST) of an initially empty undirected graph, as edges are inserted, deleted, or their weights are changed over time.

# Assumption & Requirements
    1. The graph is initially empty and undirected. Nodes are numbered from 1 to N, where N is the total number of nodes in the graph, specified in the first line of the input file ‘mst.in.’ N is less than or equal to 500.
    2. The total number of insertEdge, findMST, deleteEdge, and changeWeight oper- ations is less than or equal to 10,000.
    3. Edge weights are positive integers.
    4. If the graph is connected, findMST should print the total weight sum of the MST to the output file ‘mst.out.’ If the graph is disconnected, it should print ‘Disconnected.’
    5. If an operation is not valid (e.g., inserting an existing edge, deleting a non-existent edge), it should be ignored.

# Example Input & Output
    5
    findMST
    insertEdge 1 2 4
    insertEdge 1 3 3
    insertEdge 2 3 2
    insertEdge 3 4 1
    insertEdge 4 5 5
    findMST
    deleteEdge 2 3
    changeWeight 1 3 6
    findMST
    insertEdge 2 3 4
    findMST
    changeWeight 4 5 1
    findMST

    Expected Output (mst.out):
    Disconnected
    11
    16
    14
    10



# Overall logic
    - you have to connect all edges (if its already connected, ignore)
    - find the minimum spanning tree of all the edges
    - if the graph is disconnected, print disconnected

# Implementation
## struct??
    - node
        - int id
        - int weight
        - node *next
    - edge
        - int weight
        - node *node1
        - node *node2
        - edge *next
    - graph
        - int numNodes
        - int numEdges
        - node *nodes
        - edge *edges

## main
    1. get number of nodes from first line of file (N <= 500)
    2. loop until end of instructions in file
    3. if the instruction is findMST, call findMST
    4. otherwise, add those instructions to a list until findMST is called
    5. when mst is called reorder the MST and findMST

## set
    - make set
    - find set
    - union set

## helper functions
    - quick sort

## MST functions
### findMST

### insertEdge

### deleteEdge

### changeWeight
