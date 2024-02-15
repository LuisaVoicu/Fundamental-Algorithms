/**
*@author Voicu Laura-Luisa
*@group 30226 - semigrupa 2
*
* ***** K-Way-Merge. *****
* * Implementare.Analiza pe cazuri **
*
*/
/*
*      * * * * * * * * * * * * *
*      *  INTERPRETARE GRAFICE *
*      * * * * * * * * * * * * *
*      
* ~~> CaseI: k are valori fixe, n are valori variabile ==> n ghideaza complexitatea algoritmului
*      |--> Complexitate  ~ O(n log k) + O(k)  ~ O(n log k)  ~ O(n)
*      |   OBS: partea O(log k) "ridica" graficul mai sus 
*      |   Acest lucru este evident pentru graficul determinat cu k = 100 ce e mai indepartat  de originie fata de graficele
*      |--> spike-urile sunt cauzate de partitionarile random in cele k liste
*      |

* 
* ~~> Case2: k are valori variabile , n are valoarea fixa = 10000
*      |--> Complexitatea ~ O(n log k ) + O(k) 
*      |--> n - constant => complexitatea e data de (aproximativ) O(k + log k)
*      |--> se observa si in grafic complexitatea ce tinde sa fie liniara -> dar care e influentata de O(log k ) si n 
*                                                       |-> din nou, randomizarea dimensiunilor determina aparitia spike-urilor 
*                                                             
* 
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#define NBS_TESTS 200
#define STEP_SIZE 100
#define STEP_SIZE_K 10
#define MAX_K 500
#define MAX_N 10000

Profiler p("KWayMerge");

/*          SINGLY LINKED LIST          */
typedef struct list
{
    int index;
    int key;
    struct list* next;
}ListType;

typedef struct
{
    ListType* first;
    ListType* last;
}ListFLType;



/*          Functii Auxiliare       */

void showList(ListType* first)
{
    while (first != NULL)
    {
        printf("%2d (%d), ", first->key, first->index);
        first = first->next;
    }
    printf("\n");
}

void showHeap(ListFLType** listArray, int k)
{
    printf("\nMin Heap :\n");
    for (int i = 0; i < k; i++)
    {
        printf("%d-> ", i);
        showList(listArray[i]->first);
    }
    printf("________\n");
}

void swapping(ListFLType** a, ListFLType** b)
{
    ListFLType c = **b;
    **b = **a;
    **a = c;
}


/*          Operatii liste          */    
ListType* createList(int givenKey, int index)
{
    ListType* pointer = (ListType*)malloc(sizeof(ListType));
    if (pointer == NULL)
    {
        printf("Nu poate fi alocata memorie pentru lista!");
        exit(123);
    }
    pointer->index = index;
    pointer->key = givenKey;
    pointer->next = NULL;
}

void insertLast(ListFLType** list, int givenKey, int index, Operation* opAttr, Operation* opComp)
{
    ListType* node = createList(givenKey, index);

    (*opComp).count();
    if ((*list)->first == NULL)
    {
        (*opAttr).count();
        (*list)->first = (*list)->last = node;
        return;
    }
    (*opAttr).count(2);
    (*list)->last->next = node;
    (*list)->last = node;
}

ListType* extractFirstFromList(ListFLType** list, Operation* opAttr, Operation* opComp)
{
    (*opComp).count();
    if ((*list)->first != NULL)
    {
        (*opAttr).count(3);
        ListType* rememberAddress = (*list)->first; //(1)
        ListType* rememberList = createList(rememberAddress->key,rememberAddress->index);
        (*list)->first = (*list)->first->next; //(2)

        (*opComp).count();
        if ((*list)->first == NULL)
        {
            (*opAttr).count();
            (*list)->last = NULL;
        }
    free(rememberAddress);
    return rememberList;
    }
    return NULL;
}

bool isListSorted(ListType* first)
{
    if (first == NULL || first->next == NULL)
        return true;
    ListType* prev = first;
    first = first->next;
    while (first != NULL)
    {
        if (prev->key > first->key)
            return false;
        prev = first;
        first = first->next;
    }
    return true;
}



/*          MIN-Heap de liste       */

int getIndexOfSmallest(ListFLType** listArray, int k, int index, Operation* opAttr, Operation* opComp)
{
    // Complexitate O(1)
    int indexOfSmallest = index;
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    (*opComp).count(2);

    if (leftChild < k && listArray[indexOfSmallest]->first->key > listArray[leftChild]->first->key)
    {
        indexOfSmallest = leftChild;
    }
    if (rightChild < k && listArray[indexOfSmallest]->first->key > listArray[rightChild]->first->key)
    {
        indexOfSmallest = rightChild;
    }

    return indexOfSmallest;
}

void heapify(ListFLType** listArray, int k, int index, Operation* opAttr, Operation* opComp)
{
    // Complexitate O(log k) (k  reprezinta "numarul total" de elemente( in cazul asta LISTE) din heap
    int smallestValueIndex = getIndexOfSmallest(listArray, k, index, opAttr, opComp);
    if (smallestValueIndex != index)
    {
        (*opAttr).count(3);
        swapping(&listArray[index], &listArray[smallestValueIndex]);
        heapify(listArray, k, smallestValueIndex, opAttr, opComp);
    }
}

void buildHeap(ListFLType** listArray, int k, Operation* opAttr, Operation* opComp)
{
    // Complexitate O(k) (k  reprezinta "numarul total" de elemente( in cazul asta LISTE) din heap
    int m = k;
    for (int i = m / 2 - 1; i >= 0; i--)
    {
        heapify(listArray, k, i, opAttr, opComp);
    }
}

ListType* extractMinHeap(ListFLType** listArray,int *k, Operation* opAttr, Operation* opComp)
{
    // Complexitate O(log k) (k  reprezinta "numarul total" de elemente( in cazul asta LISTE) din heap
    ListType* min = extractFirstFromList(&listArray[0], opAttr, opComp);
    if (min == NULL)
    {
        printf("....\nnu mai sunt elemente in heap!\n");
        return NULL;
    }
    (*opComp).count();
    if (listArray[0]->first == NULL)
    {
        // nu mai avem elemente in listArray[0] -> aducem elementul de pe prima pozitie in top 
                       // si ne asiguram ca heap-ul respecta proprietatea de minHeap (Heapify)
        (*opAttr).count();
        listArray[0] = listArray[*k - 1];
        (*k)--;
    }
    heapify(listArray, *k, 0, opAttr, opComp);
    return min;

}

ListFLType* KWayMerge(ListFLType** listArray, int index, int k, int n, Operation* opAttr, Operation* opComp)
{
    /*
    * ~ How it works?
    *   Interclasarea listelor consta in "scoaterea" celui mai mic element (la nivelul tutudor listelor) din heap 
    * si adaugarea lui intr-o lista finala ce pastreaza elementele din toate listele, sortate crescator
    * 
    *   La extragerea minimului din heap are loc, de fapt, extragerea efectiva a elementului din prima lista din heap si trecerea la 
    * elementul urmator
    *   Daca lista se goleste se inlocuieste varful heapului cu ultimul element si se reface structura de heap 
    * 
    */
    ListFLType* sortedList = (ListFLType*)malloc(sizeof(ListFLType));
    sortedList->first = sortedList->last = NULL;
    buildHeap(listArray, k, opAttr, opComp); // O(k) ( nu e influentat de n pentru ca initial heapul se construieste 
                                            //tinand cont doar de primul element din fiecare lista

    while (k > 0)
    {
        ListType* min = extractMinHeap(listArray, &k, opAttr, opComp); // O(log k) (pentru un singur apel) * n ( numarul total de apeluri facute)
        if (min != NULL)                                               // ==> O(n log k)
        {
            insertLast(&sortedList, min->key, min->index, opAttr, opComp); //O(1)
        }
 
    }
    if (isListSorted(sortedList->first) == false) // Test aditional, nu tine de algoritm :)
    {
        printf("\nEroare pentru KWayMerge! Sirul nu e sortat.\n");
        return NULL;
    }

    return sortedList;

    // Complexitate totala: O(nlog k) + O( k ) 

}

ListFLType* TwoWayMerge(ListFLType* list1, ListFLType* list2, int index, int n)
{
    ListFLType* sortedList = (ListFLType*)malloc(sizeof(ListFLType));
    sortedList->first = sortedList->last = NULL;
    Operation justDemo = p.createOperation("justDemo", 0);

    while (list1->first != NULL && list2->first != NULL)
    {
        if (list1->first->key < list2->first->key)
        {
            insertLast(&sortedList, list1->first->key, list1->first->index, &justDemo, &justDemo);
            list1->first = list1->first->next;
        }
        else if(list1->first->key > list2->first->key)
        {
            insertLast(&sortedList, list2->first->key, list2->first->index, &justDemo, &justDemo);
            list2->first = list2->first->next;
        }
        else
        {
            insertLast(&sortedList, list1->first->key, list1->first->index, &justDemo, &justDemo);
            list1->first = list1->first->next;
            insertLast(&sortedList, list2->first->key, list2->first->index, &justDemo, &justDemo);
            list2->first = list2->first->next;
        }
    }

    while (list1->first != NULL)
    {
        insertLast(&sortedList, list1->first->key, list1->first->index, &justDemo, &justDemo);
        list1->first = list1->first->next;
    }
    while (list2->first != NULL)
    {
        insertLast(&sortedList, list2->first->key, list2->first->index, &justDemo, &justDemo);
        list2->first = list2->first->next;
    }

    if (isListSorted(sortedList->first) == false)
    {
        printf("\nEroare pentru TwoWayMerge! Sirul nu e sortat.\n");
        return NULL;
    }
    return sortedList;

    
}

void doKMerge(int n, int k, int caseK)
{

    int* arr = (int*)malloc(sizeof(int) * n);
    ListFLType** listArray = (ListFLType**)malloc(k * sizeof(ListFLType*));
    ListFLType* unsortedList = (ListFLType*)malloc(sizeof(ListFLType));
    unsortedList->first = NULL;
    unsortedList->last = NULL;
    
    
    int rest = n-k;
    /*
    fiecare lista are cel putin 1 element deci vor ramane n-k elemente de distribuit random prin liste
    */

    printf("Cele %d liste sunt formate din urmatoarele elemente: \n",k);
    printf("Index\tSize\t  Array\n");
    int i;
    Operation justDemo = p.createOperation("justDemo", 0);
    int size;
    int stop = 0;
       
        srand(time(NULL));
        rest = n - k;
        for (i = 0; i < k - 1 && rest; i++)
        {

            size = rand() % (rest - 0 + 1) + 0;
            rest -= size;
            size++;
            FillRandomArray(arr, size, 0, 100, false, ASCENDING);

            listArray[i] = (ListFLType*)calloc(size, sizeof(ListFLType));
            for (int j = 0; j < size; j++)
            {
                insertLast(&listArray[i], arr[j], i, &justDemo, &justDemo);
                insertLast(&unsortedList, arr[j], i, &justDemo, &justDemo);
            }
            printf("%d\t%d\t: ", i, size);
            showList(listArray[i]->first);
        }
        for (i = i; i < k; i++)
        {
            size = rest + 1;
            FillRandomArray(arr, size, 0, 100, false, ASCENDING);

            listArray[i] = (ListFLType*)calloc(size, sizeof(ListFLType));
            for (int j = 0; j < size; j++)
            {
                insertLast(&listArray[i], arr[j], i, &justDemo, &justDemo);
                insertLast(&unsortedList, arr[j], i, &justDemo, &justDemo);

            }
            printf("%d\t%d\t: ", i, rest + 1);
            showList(listArray[i]->first);
        }

    ListFLType* sortedList;

    if (caseK != 2)
    {
        Operation justDemo = p.createOperation("justDemo", 0);
        sortedList = KWayMerge(listArray, 0, k, n, &justDemo, &justDemo);
    }
    else
    {
        sortedList = TwoWayMerge(listArray[0], listArray[1], 0, n);
    }
    printf("\n________\n");
    printf("Before Merge: ");
    showList(unsortedList->first);
    printf("\nAfter Merge: ");
    showList(sortedList->first);
}

void demo()
{
    int n, k;
   
    /*      Merge 2 lists     */
    k = 2;
    printf("\n_____Interclasarea a 2 liste_____\n\n");
    do {
        printf("Introduceti valoarea lui n :");
        scanf("%d", &n);
    } while (n < k);
    doKMerge(n, k,2);

    
    /*      Merge k lists    */
    printf("\n_____Interclasarea a k liste_____\n\n");
    do {
        printf("Introduceti valoarea lui k :");
        scanf("%d", &k);
    } while (k < 1);
    do {
        printf("Introduceti valoarea lui n :");
        scanf("%d", &n);
    } while (n < k);
    doKMerge(n, k, 0);
    

}


void performanceCase1()
{
    // k constant , n variabil 

    Operation doesntCount = p.createOperation("doesntCount", 0);

    int k,n;
    int arr[3] = { 5, 10, 100 };
    const char* at = (char*)malloc(20);
    const char* co = (char*)malloc(20);
    
    srand(time(NULL));

    for (int l = 0; l < sizeof(arr) / sizeof(int); l++)
    {
        k = arr[l];
        for (n = STEP_SIZE; n <= MAX_N; n += STEP_SIZE)
        {
            int* arr = (int*)malloc(sizeof(int) * n);
            ListFLType** listArray = (ListFLType**)malloc(k * sizeof(ListFLType*));
            int rest = n - k;
            for (int nbTests = 0; nbTests < NBS_TESTS; nbTests++)
            {
                int i;
                for (i = 0; i < k - 1 && rest; i++)
                {

                    int size = rand() % (rest - 0 + 1) + 0;
                    rest -= size;
                    size++;
                    FillRandomArray(arr, size, 100, 10000, false, ASCENDING);

                    listArray[i] = (ListFLType*)calloc(size, sizeof(ListFLType));
                    for (int j = 0; j < size; j++)
                    {
                        insertLast(&listArray[i], arr[j], i,&doesntCount, &doesntCount);
                    }
                }
                for (i = i; i < k; i++)  // restul numerelor pana la k vor avea valoarea 1 ( in cazul in care raman)
                {
                    int size = rest + 1;
                    FillRandomArray(arr, size, 100, 10000, false, ASCENDING);

                    listArray[i] = (ListFLType*)calloc(size, sizeof(ListFLType));
                    for (int j = 0; j < size; j++)
                    {
                        insertLast(&listArray[i], arr[j], i, &doesntCount, &doesntCount);
                    }
                }


                if (k == 5)
                {
                    at = "Attr: k is 5 / n is variable";
                    co = "Comp: k is 5 / n is variable";
                }
                if (k == 10)
                {
                    at = "Attr: k is 10 / n is variable";
                    co = "Comp: k is 10 / n is variable";
                }
                if (k == 100)
                {
                    at = "Attr: k is 100 / n is variable";
                    co = "Comp: k is 100 / n is variable";
                }

                Operation opAttr = p.createOperation(at, n);
                Operation opComp = p.createOperation(co, n);

                KWayMerge(listArray, 0, k, n, &opAttr, &opComp);
            }
        }

        if (k == 5)
        {
            p.divideValues(at, NBS_TESTS);
            p.divideValues(co, NBS_TESTS);
            p.addSeries("kIs5", at, co);
        }
        else if (k == 10)
        {
            p.divideValues(at, NBS_TESTS);
            p.divideValues(co, NBS_TESTS);
            p.addSeries("kIs10", at, co); 
        }
        else
        {
            p.divideValues(at, NBS_TESTS);
            p.divideValues(co, NBS_TESTS);
            p.addSeries("kIs100", at, co);

        }
    }
  
   
    p.createGroup("Case1", "kIs5", "kIs10", "kIs100");
    p.createGroup("Case1Attrs", "Attr: k is 5 / n is variable", "Attr: k is 10 / n is variable", "Attr: k is 100 / n is variable");
    p.createGroup("Case1Comps", "Comp: k is 5 / n is variable", "Comp: k is 10 / n is variable", "Comp: k is 100 / n is variable");
}


void performanceCase2()
{


    int n = 10000;
    int k;
    const char* at = "Attr: k is variable and n is constant";
    const char* co = "Comp: k is variable and n is constant";
    Operation doesntCount = p.createOperation("doesntCount", 0);

    srand(time(NULL));

    for (k = STEP_SIZE_K; k <= MAX_K; k += STEP_SIZE_K)
    {
        int* arr = (int*)malloc(sizeof(int) * n);
        ListFLType** listArray = (ListFLType**)malloc(k * sizeof(ListFLType*));
        int rest = n - k;
        for (int nbTests = 0; nbTests < NBS_TESTS; nbTests++)
        {
            int i;
            for (i = 0; i < k - 1 && rest; i++)
            {

                int size = rand() % (rest - 0 + 1) + 0;
                rest -= size;
                size++;
                FillRandomArray(arr, size, 100, 10000, false, ASCENDING);

                listArray[i] = (ListFLType*)calloc(size, sizeof(ListFLType));
                for (int j = 0; j < size; j++)
                {
                    insertLast(&listArray[i], arr[j], i, &doesntCount, &doesntCount);
                }
            }
            for (i = i; i < k; i++)
            {
                int size = rest + 1;
                FillRandomArray(arr, size, 100, 10000, false, ASCENDING);

                listArray[i] = (ListFLType*)calloc(size, sizeof(ListFLType));
                for (int j = 0; j < size; j++)
                {
                    insertLast(&listArray[i], arr[j], i ,&doesntCount, &doesntCount);
                }
            }

            Operation opAttr = p.createOperation(at, k);
            Operation opComp = p.createOperation(co, k);

            KWayMerge(listArray, 0, k, n, &opAttr, &opComp);
        }
    }

    p.divideValues(at, NBS_TESTS);
    p.divideValues(co, NBS_TESTS);
    p.addSeries("Operations: k is variable and n is constant", at, co);
    p.createGroup("Case2AttrsAndComps", at, co);
    p.createGroup("Case2", "Operations: k is variable and n is constant");


}


void performanceAll()
{
    p.reset("Case1");
    performanceCase1();
    p.reset("Case2");
    performanceCase2();
    p.showReport();
}
int main()
{


    demo();
    //performanceAll();

}
