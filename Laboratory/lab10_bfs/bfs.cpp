#include <stdlib.h>
#include <string.h>
#include "bfs.h"
#define  INF 1000000




// queue


ListNode* makeListNode(Node* key)
{
    ListNode* q = (ListNode*)malloc(sizeof(ListNode));
    q->key = key;
    q->next = NULL;
    return q;
}

void enqueue(Queue* q, Node* key)
{
    ListNode* node = makeListNode(key);
    if (q->head == NULL)
    {
        q->head = q->tail = node;
        return;
    }
    q->tail->next = node;
    q->tail = node;
}


Node* dequeue(Queue* q)
{
    if (q->head == NULL)
    {
        return NULL;
    }

    Node* remember = q->head->key;
    q->head = q->head->next;
    if (q->head == NULL)
    {
        q->tail = NULL;
    }
    return remember;
}


// stack




Stack* makeStackNode(int key, Node* node)
{
    Stack* S = (Stack*)malloc(sizeof(Stack*));
    S->pair.key = key;
    S->pair.node = node;
    S->next = NULL;
    return S;
}


Pair pop(Stack** stack)
{
    if ((*stack) == NULL)
    {
        return { -1,NULL };
    }

    Pair remember = (*stack)->pair;
    Stack* node = *stack;
    (*stack) = (*stack)->next;
    //free(node);
    return remember;

}

void push(Stack** stack, int key, Node* node)
{
    Stack* nodeS = makeStackNode(key,node);
    if ((*stack) == NULL)
    {
        *stack = nodeS;
        return;
    }
    
    nodeS->next = (*stack);
    (*stack) = nodeS;
}



/////////////////////////////////////////////


int valid(const Grid* grid, Point p)
{
    if (p.col >= grid->cols || p.col < 0 || p.row >= grid->rows)
    {
        return 0;
    }
    if (grid->mat[p.row][p.col] != 0)
    {
        return 0;
    }
    return 1;
}


///////////////////////////////////////////


int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    // DONE
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4

      int neighY[4] = {0,0,-1,1};
      int neighX[4] = {-1,1,0,0 };
    int countNeighb = 0;

    for (int i = 0; i < sizeof(neighY) / sizeof(int); i++)
    {
        Point newPoint;
        newPoint.col = p.col + neighY[i];
        newPoint.row = p.row + neighX[i];
        if (valid(grid, newPoint) != 0)
        {
            neighb[countNeighb].col = newPoint.col;
            neighb[countNeighb++].row = newPoint.row;
        }
    }

    return countNeighb;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph *graph, Node *s, Operation *op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();


    // 0 = WHITE , 1 = GRAY , 2 = BLACK

    for (int i = 0; i < graph->nrNodes; i++)
    {
        if (op != NULL) op->count();
        graph->v[i]->color = 0;
        graph->v[i]->dist = INF;
        graph->v[i]->parent = NULL;
    }
    if (op != NULL) op->count(); // comp din for

    if (op != NULL) op->count();
    s->color = 1;
    s->dist = 0;
    Queue Q = { NULL,NULL };
    enqueue(&Q, s);

    while (Q.head != NULL)
    {
        if (op != NULL) op->count();
        Node* u = dequeue(&Q);

        //printf("bfs: %d \n", u->key);
        if (u != NULL)
        {
            for(int i = 0; i < u->adjSize; i++)
            {
                if (op != NULL) op->count(); // comp din for

                if (op != NULL) op->count();
                Node* vertex = u->adj[i];

                if (vertex->color == 0)
                {
                    //printf("%d %d \n", u->key, vertex->key);
                    vertex->color = 1;
                    vertex->dist = u->dist + 1;
                    vertex->parent = u;
                    enqueue(&Q, vertex);
                }
            } 
           if (op != NULL) op->count(); // comp din for


        if (op != NULL) op->count();
        u->color = 2;
        }
       // printf("\n\n");

    }
    if (op != NULL) op->count();

}

void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs

        int root;
        for (int i = 0; i < n; i++)
        {
            if (p[i] == -1)
            {
                root = i;
            }
        }
          
        int parent = root;
        
        Stack* S = NULL;
        push(&S, root,NULL);

        int* spacing = (int*)calloc((n+1) , sizeof(int));
        spacing[root] = 0;
        
        while (S)
        {
            int currentParent = pop(&S).key;
            if (currentParent == -1)break;
            for (int i = 0; i < spacing[currentParent]; i++)
            {
                printf("\t");
            }
            
            printf("(%d , %d)\n", repr[currentParent].row, repr[currentParent].col);
            for(int i = n - 1 ; i >= 0 ; i--)
                if (p[i] == currentParent)
                {
                    spacing[i] = spacing[currentParent] + 1;
                    push(&S, i, NULL);
                }
        }

    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
    printf("%d %d\n", start->position.row,start->position.col);

    int sizePath = 0;
    bfs(graph, start, NULL);

    while (end != start)
    {
        path[sizePath++] = end;
        end = end->parent;
    }
    path[sizePath++] = start;



    for (int i = 0; i < sizePath / 2; i++)
    {
        Node* aux = path[i];
        path[i] = path[sizePath - i - 1];
        path[sizePath - i - 1] = aux;
    }
   /* printf("\n\n");

    for (int i = 0; i < sizePath; i++)
    {
        printf("%d %d\n", path[i]->position.row, path[i]->position.col);

    }*/

    return sizePath;
}



///// performance
Node* make_node_from_key(int key)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->adj = NULL;
    return node;
}
int already_edge(Node*** adjList, int size, int u, int v)
{
    if (size <= 0)
        return 1;
    for (int i = 0; i < size; i++)
    {
        if (adjList[u][i]->key == v)
        {
            return 0;
        }
    }
    return 1;
}
Graph generata_random_graph_with_keys(Graph* graph, int nbVertices, int nbEdges)
{
    graph->nrNodes = nbVertices;

    int* vert = (int*)malloc((graph->nrNodes + 1) * sizeof(int));
    FillRandomArray(vert, graph->nrNodes, 0, graph->nrNodes - 1, true, UNSORTED);


    int* sizeAdj = (int*)calloc(nbVertices + 1, sizeof(int));
    Node*** adjList = (Node***)malloc((nbVertices + 1)* sizeof(Node**));
    for (int i = 0; i < nbVertices; i++)
    {
        adjList[i] = (Node**)calloc((nbVertices + 1), sizeof(Node*));
        graph->v[i] = make_node_from_key(i);
    }

    /*
    for (int i = 1; i < nbVertices; i++)
    {
        printf("  %d %d\n", vert[i - 1], vert[i]);
    }*/

    for (int i = 1; i < graph->nrNodes; i++)
    {

        //sizeAdj[vert[i - 1]]++;
        
        adjList[vert[i - 1]][sizeAdj[vert[i - 1]]++] = graph->v[vert[i]]; // inclusa alocarea
        //printf("abc %d %d\n", vert[i - 1], sizeAdj[vert[i - 1]]);
    }


    for (int i = nbVertices; i <= nbEdges; i++)
    {
        int u, v;
        do
        {
            u = rand() % (nbVertices - 1 - 0 + 1);
            do
            {
                v = rand() % (nbVertices - 1 - 0 + 1);
            } while (u == v);
        } while (already_edge(adjList, sizeAdj[u], u, v) == 0);


        adjList[u][sizeAdj[u]++] = graph->v[v];

      //  printf("x %d %d\n", u, v);
    }

    for (int i = 0; i < nbVertices; i++)
    {
        graph->v[i]->adjSize = sizeAdj[i];
        graph->v[i]->adj = adjList[i];
    }

    return *graph;
}

void performance()
{
    srand(time(NULL));

    int n, i;
    Profiler p("bfs");

    // testare generare graf random

   /* Operation op = p.createOperation("test_only", 0);
    Graph graph;
    graph.nrNodes = 6;
    graph.v = (Node**)malloc((graph.nrNodes+1) * sizeof(Node*));

    for (i = 0; i < graph.nrNodes; ++i) {
        graph.v[i] = (Node*)malloc(sizeof(Node));
        memset(graph.v[i], 0, sizeof(Node));
    }

    graph = generata_random_graph_with_keys(&graph, 6, 12);


    for (int i = 0; i < graph.nrNodes; i++)
    if(graph.v[i]->adjSize>0) {
        printf(" %d -->",  i);
        for (int j = 0; j < graph.v[i]->adjSize; j++)
        {
            int u = graph.v[i]->adj[j]->position.row;
            int v = graph.v[i]->adj[j]->key;
            printf(" %d ", v);
        }
        printf("\n");
    }


    bfs(&graph, graph.v[0], &op);
    for (int i = 0; i < 10000000000; i++);*/
    
    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        graph = generata_random_graph_with_keys(&graph, graph.nrNodes, n);
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for(n=100; n<=200; n+=10){
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        graph = generata_random_graph_with_keys(&graph, n, 4500);
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }
    
    p.showReport();
}


/*
Node* make_node_from_position(Point p)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->position = p;
    node->adj = NULL;
    return node;
}

int valid_knight_position(Grid* grid, Point p)
{
    if (p.row >= grid->rows || p.col >= grid->cols || p.row < 0 || p.col < 0)
    {
        return 0;
    }
    if (grid->mat[p.row][p.col] != 0)
    {
        return 0;
    }
    return 1;

}

void knight_moves(Grid* grid, Node* s)
{

    s->color = 1;
    Queue Q = { NULL,NULL };
    enqueue(&Q, s);
    int mark = 1;
    grid->mat[s->position.row][s->position.col] = mark;
    while (Q.head)
    {
        Node* u = dequeue(&Q);

        int positionX[8] = { -1,-1,1,1, -2,-2,2,2 };
        int positionY[8] = { -2,2,2,-2, -1,1,1,-1 };

        for (int k = 0; k < 8; k++)
        {
            Point newPos;
            newPos.row = u->position.row + positionX[k];
            newPos.col = u->position.col + positionY[k];
            if (valid_knight_position(grid, newPos))
            {
                for (int i = 0; i < grid->rows; i++)
                {
                    for (int j = 0; j < grid->cols; j++)
                    {
  
                        printf("%d ", grid->mat[i][j]);
                    }
                    printf("\n");
                }
                printf("\n\n");
                grid->mat[newPos.row][newPos.col] = ++mark;
                Node* node = make_node_from_position(newPos);
                enqueue(&Q, node);
                sleep(1);
                
            }

        }
    }


}
*/