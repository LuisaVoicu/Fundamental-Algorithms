#ifndef __BFS_H__
#define __BFS_H__

#include "Profiler.h"

#define MAX_ROWS 100
#define MAX_COLS 100

typedef struct{
    int rows;
    int cols;
    int mat[MAX_ROWS][MAX_COLS];
}Grid;

typedef struct{
    int row;
    int col;
}Point;

enum{
    COLOR_WHITE = 0,
    COLOR_GRAY,
    COLOR_BLACK
};

typedef struct _Node{
    int key; 
    Point position;
    int adjSize;
    struct _Node **adj;

    int color;
    int dist;
    struct _Node *parent;
}Node;

typedef struct{
    int nrNodes;
    _Node **v;
}Graph;

typedef struct _ListNode
{

    Node* key;
    _ListNode* next;
}ListNode;

typedef struct 
{
    _ListNode* head;
    _ListNode* tail;
}Queue;


typedef struct Pair
{
    int key;
    Node* node;
};
typedef struct _Stack {
    Pair pair;
    _Stack* next;
}Stack;




ListNode* makeListNode(Node* key);
void enqueue(Queue* q, Node* key);
Node* dequeue(Queue* q);

_Stack* makeStackNode(int key);
Pair pop(Stack** stack);
void push(Stack** stack, int key, Node* node);

int get_neighbors(const Grid *grid, Point p, Point neighb[]);
void grid_to_graph(const Grid *grid, Graph *graph);
void free_graph(Graph *graph);
void bfs(Graph *graph, Node *s, Operation *op=NULL);
void print_bfs_tree(Graph *graph);
int shortest_path(Graph *graph, Node *start, Node *end, Node *path[]);
void performance();
//void knight_moves(Grid* grid, Node* s);
//Node* make_node_from_position(Point p);

#endif