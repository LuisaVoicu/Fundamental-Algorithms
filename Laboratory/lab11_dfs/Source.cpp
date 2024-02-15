/**
*@author Voicu Laura-Luisa
*@group 30226 - semigrupa 2
*
* DFS. Sortare Topologica. Determinarea Componentelor Tare Conexe cu Algoritmul lui Tarjan
* -- Implementare. Analiza complexitate
*
*/

/*
COMPLEXITATI

DFS: O(V+E)
Sortare Topologica: O(V+E) --> DFS( - O(V+E)) + push Stack (O(1))
Tarjan : ~ similar DFS --> TARJAN -> V  ~~ pt fiecare V se apeleaza STRONG-CONNECT -> E overall (pt toate nodurile) ==> O(V + E)


// Analiza grafice: 
V  constant (=100) => E variabil => DFS(V+E) = DFS(V+100) -- liniar
V  variabil => E constant(=4500) => DFS(V+E) = DFS(V+4500) -- liniar
*/

#define _CRT_SECURE_NO_WARNINGS
#include "Profiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 4500

typedef enum
{
    WHITE,
    GRAY,
    BLACK
}Color;


typedef struct _Node
{
    int index;
    int lowLink;
    bool onStack;
    int comp;

    int key;
    int color;
    int discover;
    int finish;
    int adjSize;
    _Node* parent;
    _Node** adjList;

}Node;

typedef struct _Graph
{
    int E, V;
    Node** vertices;
}Graph;

typedef struct _ListNode
{
    Node* key;
    _ListNode* next;
}ListNode;

int minim(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

ListNode* createListNode(Node* key)
{
    ListNode* listNode = (ListNode*)malloc(sizeof(ListNode*));
    listNode->key = key;
    listNode->next = NULL;
    return listNode;
}

void insertFirst(ListNode** list, Node* key)
{
    ListNode* listNode = createListNode(key);
    if (*list == NULL)
    {
        *list = listNode;
        return;
    }

    listNode->next = *list;
    *list = listNode;
}

Node* deleteFirst(ListNode** list)
{
    // dezalocare?
    if (*list == NULL)
    {
        return NULL;
    }
    Node* remember = (*list)->key;
    *list = (*list)->next;
    return remember;
}

void showListNode(ListNode* list)
{
    printf("\n\nSortare Topologica: ");
    while (list != NULL)
    {
        printf("%d ", list->key->key);
        list = list->next;
    }
    printf("\n");

}



Node* createNodeGraph(int key, int nbRows, int nbCols)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->adjList = (Node**)calloc(nbRows + 1, sizeof(Node*));
    for (int i = 0; i < nbRows; i++)
    {

        node->adjList[i] = (Node*)calloc(nbCols + 1, sizeof(Node));
    }
    node->adjSize = 0;
    return node;
}

Graph buildGraphFromInputTxt()
{
   FILE* pf = fopen("graph.txt", "r");

    Graph graph;

    fscanf(pf, "%d %d", &graph.V, &graph.E);

    for (int i = 0; i < graph.V; i++)
    {
        graph.vertices = (Node**)malloc(graph.V * sizeof(Node*));
    }

    for (int i = 0; i < graph.V; i++)
    {
        graph.vertices[i] = createNodeGraph(i, graph.V, graph.E);
    }


    for (int i = 0; i < graph.E; i++)
    {
        int u, v;
        fscanf(pf, "%d %d", &u, &v);
        graph.vertices[u]->adjList[graph.vertices[u]->adjSize++] = graph.vertices[v];
    }
    fclose(pf);
    return graph;
}



Node* make_node_from_key(int key)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->adjSize = 0;
    return node;
}

int already_edge(Graph* graph, int u, int v)
{

    for (int i = 0; i < graph->vertices[u]->adjSize; i++)
    {
        if (graph->vertices[u]->adjList[i]->key == v)
        {
            return 0;
        }
    }
    return 1;
}

Graph generateRandomGraph(Graph* graph, int vertices, int edges)
{
    graph->V = vertices;
    graph->E = edges;

    for (int i = 0; i < vertices; i++)
    {
        graph->vertices[i] = make_node_from_key(i);
        graph->vertices[i]->adjList = (Node**)malloc((vertices + 1) * sizeof(Node*));
        
    }

    for (int i = 0; i <= edges; i++)
    {
        int u, v;
        do
        {
            u = rand() % (vertices - 1 - 0 + 1);
            do
            {
                v = rand() % (vertices - 1 - 0 + 1);
            } while (u == v);
        } while (already_edge(graph, u, v) == 0);
        graph->vertices[u]->adjList[graph->vertices[u]->adjSize++] = graph->vertices[v];

    }

    return *graph;

}

void freeGraph(Graph* graph)
{
    if (graph->vertices != NULL)
    {
        for (int i = 0; i < graph->V; i++)
        {
            if (graph->vertices[i] != NULL)
            {
                if (graph->vertices[i]->adjList != NULL)
                {
                    free(graph->vertices[i]->adjList);
                    graph->vertices[i]->adjList = NULL;
                }
                graph->vertices[i]->adjSize = 0;
                free(graph->vertices[i]);
                graph->vertices[i] = NULL;
            }
        }
        free(graph->vertices);
        graph->vertices = NULL;
    }
    graph->V = 0;
    graph->E = 0;
}

void showGraph(Graph graph)
{
    for (int i = 0; i < graph.V; i++)
    {
        printf("%d : ",i);
        for (int j = 0; j < graph.vertices[i]->adjSize; j++)
            printf("%d ", graph.vertices[i]->adjList[j]->key);
        printf("\n");
    }
}

void DFSVisit(Node* node, int* timer, ListNode** list, bool showVertex, Operation* op)
{
    if (op != NULL) (*op).count();
    node->discover = ++(*timer);
    node->color = GRAY;

    if (showVertex == true)
    {
        printf("%d ", node->key);
    }
    for (int i = 0; i < node->adjSize; i++)
    {
        if (op != NULL) (*op).count(2);
        if (node->adjList[i]->color == WHITE)
        {
            node->adjList[i]->parent = node;
            DFSVisit(node->adjList[i], timer, list, showVertex, op);
        }
    }

    node->color = BLACK;
    node->finish = ++(*timer);
    //printf("%d %d %d\n", node->key, node->discover, node->finish);

    if (op != NULL) (*op).count();
    insertFirst(list, node);

}

ListNode* DFS(Graph* graph, bool showVertex, Operation* op)
{

    ListNode* list = { NULL };
    for (int i = 0; i < graph->V; i++)
    {
        if (op != NULL) (*op).count();
        graph->vertices[i]->color = WHITE;
        graph->vertices[i]->parent = NULL;
    }
    int timer = 0;
    for (int i = 0; i < graph->V; i++)
    {
        if (op != NULL) (*op).count();

        if (graph->vertices[i]->color == WHITE)
        {
            DFSVisit(graph->vertices[i], &timer, &list, showVertex, op);
            if (showVertex == true)
            {
                printf("\n");
            }
        }
    }

    return list;
}




int index;
ListNode* stack;
int nbComponents;

void StrongConnect(Graph graph, Node* u)
{
    u->index = u->lowLink = index++;
    insertFirst(&stack, u);
    u->onStack = true;

    for (int i = 0; i < u->adjSize; i++)
    {
        if (u->adjList[i]->index == -1)
        {
            StrongConnect(graph, u->adjList[i]);
            u->lowLink = minim(u->adjList[i]->lowLink, u->lowLink);
        }
        else if (u->adjList[i]->onStack == true)
        {
            u->lowLink = minim(u->adjList[i]->index, u->lowLink);
        }
    }

    if (u->lowLink == u->index)
    {
        nbComponents++;
        Node* v;
        do
        {
            v = deleteFirst(&stack);
            v->onStack = false;
            v->comp = nbComponents;
        } while (v != u);
    }
}

void Tarjan(Graph graph)
{
    for (int i = 0; i < graph.V; i++)
    {
        graph.vertices[i]->index = graph.vertices[i]->lowLink = -1;
        graph.vertices[i]->onStack = false;
        graph.vertices[i]->comp = 0;
    }
    for (int i = 0; i < graph.V; i++)
    {
        if (graph.vertices[i]->index == -1)
        {
            StrongConnect(graph, graph.vertices[i]);
        }
    }
}

void demoDFS()
{
    printf("Arbori DFS:\n");
    Graph graph = buildGraphFromInputTxt();
    //showGraph(graph);
    ListNode* list = DFS(&graph, true, NULL);
    //showListNode(list);
}

void demoTarjan()
{
    Graph graph = buildGraphFromInputTxt();
    //showGraph(graph);
    Tarjan(graph);
    for (int i = 0; i < graph.V; i++)
    {
        printf("%d %d\n", i, graph.vertices[i]->comp);
    }
}

void demoTopologicalSorting()
{
    Graph graph = buildGraphFromInputTxt();

    ListNode* list = DFS(&graph, false, NULL);
    showListNode(list);
}

void demoAll()
{
    Graph graph = buildGraphFromInputTxt();
    printf("\n\nGraf initial:\n");
    showGraph(graph);

    printf("\nArbori DFS: ");
    ListNode* list = DFS(&graph, true, NULL);
    showListNode(list);


    Tarjan(graph);
    printf("\n\nLista componentelor tare conexe:\n");
    for (int k = 1; k <= nbComponents; k++)
    {
        printf("C%d: ", k);
        for (int i = 0; i < graph.V; i++)
        {
            if (graph.vertices[i]->comp == k)
            {
                printf("%d ", i);
            }
        }
        printf("\n");
    }

}



void performance()
{
    Profiler prof("DFS");
    int v = 100;
    int e;
    for (e = 1000; e <= 4500; e += 100)
    {

        Operation op = prof.createOperation("DFS_Edges", e);
        Graph graph;
        graph.V = v;
        graph.E = e;

        graph.vertices = (Node**)malloc((graph.V+1) * sizeof(Node*));

        for (int i = 0; i < graph.V; i++)
        {
            graph.vertices[i] = (Node*)malloc(sizeof(Node));
        }
        graph = generateRandomGraph(&graph,v, e); 
        ListNode* list = DFS(&graph, false, &op);
        /*if(list!=NULL)
        free(list);*/
         
        freeGraph(&graph);
     
    }


    e = 4500;
    for (v = 100; v <= 200; v += 10)
    {

        Operation op = prof.createOperation("DFS_Vertices", v);
        Graph graph;
        graph.E = e;
        graph.V = v;
        graph.vertices = (Node**)malloc((graph.V + 1) * sizeof(Node*));

        for (int i = 0; i < graph.V; i++)
        {
            graph.vertices[i] = (Node*)malloc(sizeof(Node));
        }
        graph = generateRandomGraph(&graph, v, e);
        ListNode* list = DFS(&graph, false, &op);

        freeGraph(&graph);
    }
    prof.showReport();
}

int main()
{
    srand(time(NULL));
    //demoDFS();
    //demoTopologicalSorting();
    //demoTarjan();
    demoAll();
    //performance();
}