/*
*@author Voicu Laura-Luisa
*@group 30226 - semigrupa 2
*
*
*                           ***** DISJOINT SETS (WITH HEURISTICS) . KRUSKAL *****
*                           
*     ** EVALUATION OF OPERATIONS FOR DISJOINT SETS USING KRUSKAL'S ALGORITHM **
*               ***** GENERATE RANDOM TREE. GENERATE RANDOM GRAPH *****
* 
*
*
************************************************************************

                         RUNNING TIME

                                   MAKE-SET  : O(1)
 FIND-SET (with PATH COMPRESSION heuristic)  : O(1)
        UNION (with UNION BY RANK heuristic) : O(log n) 



KRUSKAL: Analiza complexitate => MAKE_SET ( for each vertex)         => v*O(1) = O(V)
                              => Sort edges by weight                => O(E log E)
                              => Building Minimum Spanning Tree      => O(alpha(v)) <-> alpha(v) ~ O(1)
                  - for each vertex FIND-SET (u,v) + UNION(u,v)
                                                                     ________________________(+) 
                                                                     => OVERALL O(E log E) ->  
                                
          CONCLUZIE: complexitatea algoritmului Kruscal pentru determinarea MST e data de metoda de sortare folosita ( cea mai optima O(E logE) )
                                                            unde WORST CASE ( la graf NEORIENTAT)  avem V(V-1)/2 muchii (graf complet neorientat)
                                                            DECI pentru WORST CASE (V = n) Complexitatea KRUSKAL = O( N(N-1)/2 * log (N/(N-1)/2) ~ O( N^2 log(N^2) ) 
                                                                                                                 = O(N^2 * 2 log N) ~ O(N^2 log N)
                                                                                                                 = O(N^2 * log N) (WORST CASE FOR NB OF EDGES)




                                                                                                                 


                                                                                                                

Cum functioneaza Kruskal?
-> foloseste multimi de paduri disjuncte (+ sortarea crescatoare a muchiilor) pentru a alege muchiile minime ce determina un arbore
   --> porneste de la muchia minima -> implicit cele 2 noduri ce formeaza muchia respectiva e adaugata in multimea disjuncta
   --> in continuare vor fi selectate acele muchii pentru care nodurile ce o formeaza nu se afla in aceeasi multime, dupa care cele 2 se vor reuni 
                                                                                                                     => lucru ce asigura faptul ca in multimea finala vom avea doar cele n noduri neduplicate

   --> OBS: arborele nu are radacina



----
Cum generez graful random pentru partea de performanta a algoritmului?
---> generez un arbore random  ==> PENTRU A ASIGURA CONEXITATEA GRAFULUI!!!
                    (p1) ->generez random un array ce reprezinta o partitie a sirulului (1,2,..,n)
                    (p2) -> pentru nodul al i-lea din sir stabilesc RANDOM care este tatal lui -> pot fi nodurile de pe indecsii 1,2..i-1
                    (p3) -> repet (p2) pentru fiecare nod posibil
--> completez arborele cu multimi random de noduri, asigurandu-ma ca nu voi da niciodata de o muchie deja existenta (suficient sa parcurg sigul de muchii --> functia AlreadyEdge )
                                                                           

*/


#define _CRT_SECURE_NO_WARNINGS
#include "Profiler.h"
#include "QuickSort.h"
#include "HeapSort.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10000
#define MAX_WEIGHT 10000
#define NB_TESTS 5
#define STEP_SIZE 100

Profiler prof("MST_Kruskal");





typedef struct DisjointSet
{
    int key;
    int rank;
    int rep;
    DisjointSet* parent;
}DisjointSet;

typedef struct 
{
    int size;
    Edges edges;
}MST;


/*          OPERAATII MULTIMI DISJUNCTE            */    

void MakeSet(DisjointSet*  x, int key, Operation* op)
{
    (*op).count();
    x->parent = (DisjointSet*)malloc(sizeof(DisjointSet));
    x->key = key;
    x->parent = x;
    x->rep = key;
    x->rank = 0;
}

DisjointSet* FindSet(DisjointSet*  x, Operation* op) 
{
    (*op).count();
    if (x != x->parent) // euristica Path compression
    {
        (*op).count(2); 
        x->rep = x->parent->rep;
        x->parent = FindSet(x->parent, op); //path compression --> facem legatura directa cu elementul reprezentativ
    }
    return x->parent;

}

void Link(DisjointSet* x, DisjointSet* y, Operation* op)
{
    // euristica UNION BY RANK
    (*op).count();
    if (x->rank > y->rank)
    {
        (*op).count(2);
        y->parent = x;
        y->rep = x->rep;
    }
    else
    {
        (*op).count(2);
        x->parent = y;
        x->rep = y->rep;

        (*op).count();
        if (x->rank == y->rank)
        {
            (*op).count();
            y->rank++;
        }
    }
}

void Union(DisjointSet* x, DisjointSet* y, Operation* op)
{
    Link(FindSet(x, op), FindSet(y, op), op); // union by rank
}



/*          FUNCTII AUXILIARE           */

void ShowMST(MST* edges, int m)
{
    printf("\n\n__________MST_______________\n");
    for (int i = 0; i < m; i++)
    {
        printf("%d %d %d\n", edges[i].edges.u, edges[i].edges.v, edges[i].edges.w);
    }
    printf("_________________________\n\n");


}

void ShowGraph(Edges* edges, int m)
{
    printf("\n\n___________GRAPH_____________\n");
    for (int i = 0; i < m; i++)
    {
        printf("%d %d %d\n", edges[i].u, edges[i].v, edges[i].w);
    }
    printf("_________________________\n\n");

}

int IsSortByWeight(Edges* edges, int m)
{
    Edges* copy = (Edges*)malloc((m) * sizeof(Edges));
    for (int i = 0; i < m; i++)
    {
        copy[i] = edges[i];
    }
    for (int i = 0; i < m - 1; i++)
    {
        for (int j = i + 1; j < m; j++)
        {
            if (copy[i].w > copy[j].w)
            {
                Edges aux = copy[i];
                copy[i] = copy[j];
                copy[j] = aux;
            }
        }
    }

    for (int i = 0; i < m; i++)
        if (edges[i].w != copy[i].w)
            return 0;
    return 1;
}


/*          KRUSKAL         */    

MST* Kruskal(Edges* edges, int n, int m, Operation* op)
{
    MST* A = (MST*)malloc(sizeof(MST) * (MAX_SIZE));
    A->size = 0;
    DisjointSet* dis = (DisjointSet*)malloc((n + 2) * sizeof(DisjointSet));
    for (int i = 1; i <= n; i++)
    {
        MakeSet(&dis[i], i, op);
    }

    // sortare edges increasing order by weight w

    quickSortHybridization(edges, 0, m - 1);
    //heapSort(edges, m);

    /*if (!IsSortByWeight(edges, m))
    {
        printf("\n----------------->NU E SORTAT EROARE!!!\n");
        exit(124);
     }*/

    int countEdges = 0;
    for (int i = 0; i < m; i++)
    {
        if (FindSet(&dis[edges[i].u], op) != FindSet(&dis[edges[i].v], op))
        {
            A[A->size++].edges = edges[i];
            //printf("in kruskal: %d %d %d\n", edges[i].u, edges[i].v, edges[i].w);
            Union(&dis[edges[i].u], &dis[edges[i].v], op);

            if (++countEdges == n - 1) // optimizare ---> MST are n-1 noduri
            {
               break;
            }
            
        }
    }

    return A;
}



/*          GENERARE GRAF RANDOM             */    

Edges* GenerateRandomTree(int n)
{
    /*
    cum?

    (p1) -> ma folosesc de profiler si generez random un array ce reprezinta o partitie a sirulului (1,2,..,n)
    (p2) -> pentru nodul al i-lea din sir stabilesc RANDOM care este tatal lui -> pot fi nodurile de pe indecsii 1,2..i-1
    (p3) -> repet (p2) pentru fiecare nod posibil

    */

    Edges* edges = (Edges*)malloc((n * 4 + 1)*sizeof(Edges));
    int maxWeight = MAX_WEIGHT;
    int* v = (int*)malloc(sizeof(int) * MAX_SIZE);
   
    FillRandomArray(v, n, 1, n, true, UNSORTED);

    for (int i = 1; i < n; i++)
    {
        // upper = i-1 , lower = 0
        int index = rand() % (i - 1 - 0 + 1) + 0;
        int weight = rand() % (maxWeight + 1) + 1;

        //printf(" %d cu p %d si w = %d\n", v[i], v[index], weight);
        edges[i - 1].u = v[index];
        edges[i - 1].v = v[i];
        edges[i - 1].w = weight;
    }
    return edges;
}

int AlreadyEdge(Edges* edge, int size, int u, int v)
{
    for (int i = 0; i < size; i++)
    {
        if ((edge[i].u == u && edge[i].v == v) ||
            (edge[i].u == v && edge[i].v == u))
        {
            return 0;
        }
    }
    return 1;
}

Edges* GenerateRandomGraph(int n, int m)
{
    // n*4 muchii va avea graful ==> asta inseamna obligatoriu ca n >= 9 ---> din n * 4 >= n(n-1)/2
    /*
    (p1) -> cum asigur conexivitatea grafului?
             => generez un arbore ----> deci folosesc n - 1 muchii
    (p2) -> raman n*4 - ( n - 1 ) muchii gasit ... cum procedez?
             => aleg random 2 muchii si verific daca nu cumva ele se regasesc deja prin array-ul de muchii sau daca am generat 2 noduri identice
    */

    Edges* edges = GenerateRandomTree(n);
    int maxWeight = MAX_WEIGHT;

    for (int i = n - 1; i < m; i++)
    {
        int u, v, w;
        w = rand() % ( maxWeight + 1 ) + 1;
        do
        {
            u = rand() % n + 1;
            do 
            {
                v = rand() % n + 1;
            } while (v == u);

        } while (AlreadyEdge(edges, i, u, v) == 0);
        edges[i].u = u;
        edges[i].v = v;
        edges[i].w = w;
    }
    return edges;

}




/*        DEMO        */
void demoDisjointSets()
{
    DisjointSet x[10];
    Operation op = prof.createOperation("JustDemo", 0);
    int keys[] = { 2, 3, 6, 9, 12, 1, 7, 4, 5, 11 };
    for (int i = 0; i < sizeof(keys) / sizeof(int); i++)
    {
        MakeSet(&x[i], keys[i], &op);
    }

    Union(&x[0], &x[1], &op);
    Union(&x[1], &x[2], &op);
    Union(&x[5], &x[6], &op);
    Union(&x[3], &x[2], &op);
    Union(&x[6], &x[2], &op);
    Union(&x[4], &x[7], &op);
    Union(&x[7], &x[9], &op);

    printf("%d il are reprezentant pe: %d\n", x[0].key, x[0].rep);
    printf("%d il are reprezentant pe: %d\n", x[2].key, x[2].rep);
    printf("%d il are reprezentant pe: %d\n", x[3].key, x[3].rep);
    printf("%d il are reprezentant pe: %d\n", x[5].key, x[5].rep);
    printf("%d il are reprezentant pe: %d\n", x[6].key, x[6].rep);
    printf("%d il are reprezentant pe: %d\n", x[5].key, x[5].rep); // aici inca nu apuca sa faca path compression
    printf("%d il are reprezentant pe: %d\n", x[9].key, x[9].rep);
    printf("%d il are reprezentant pe: %d\n", x[8].key, x[8].rep);

    DisjointSet* a = FindSet(&x[5], &op);

    printf("FindSet( %d ) = %d\n", x[5].key, a->key);
    printf("Path compression : %d il are reprezentant pe: %d\n", x[5].key, x[5].rep);
   
    a = FindSet(&x[9], &op);
    printf("FindSet( %d ) = %d\n", x[9].key, a->key); 

    a = FindSet(&x[3], &op);
    a = FindSet(&x[3], &op);
    printf("FindSet( %d ) = %d\n", x[3].key, a->key);

}

void demoForKruskal()
{

    FILE* pf;
    pf = fopen("inputs.txt", "r");

    int n, m;
    fscanf(pf, "%d %d", &n, &m);
    Edges* edges = (Edges*)malloc(sizeof(Edges) * (m + 1));

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        fscanf(pf, "%d %d %d", &u, &v, &w);
        //printf("%d %d %d\n", u, v, w);
        edges[i].v = v;
        edges[i].u = u;
        edges[i].w = w;
    }
    ShowGraph(edges, m);
    Operation op = prof.createOperation("JustDemo", 0);
    MST* mst = Kruskal(edges, n, m, &op);
    ShowMST(mst, n-1);

}

void demoMSTForRandomGraphKruskal(int n)
{
    int m = n * 2;
    Edges* edges = GenerateRandomGraph(n,m);
    ShowGraph(edges, m);
    Operation op = prof.createOperation("JustDemo", 0);
    MST* mst = Kruskal(edges, n, m, &op);
    ShowMST(mst, n - 1);
}


/*      PERFORMANCE*/    
void performance()
{
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        int m = n * 4;
        Operation op = prof.createOperation("operations", n);
        for (int nbTst = 0; nbTst < NB_TESTS; nbTst++)
        {
            Edges* edges = GenerateRandomGraph(n, m);
            MST* mst = Kruskal(edges, n, m, &op);
            free(edges);
        }

    }
    prof.divideValues("operations", NB_TESTS);
    prof.showReport();

}

int main()
{
    srand(time(NULL));
    //demoDisjointSets();
    //demoForKruskal();
    //demoMSTForRandomGraphKruskal(7);
    performance();
}