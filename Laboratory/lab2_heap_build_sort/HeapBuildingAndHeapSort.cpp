/**
*@author Voicu Laura-Luisa
*@group 30226 - semigrupa 2
*
* SORT_HEAP . BUILD_HEAP (BOTTOM UP & TOP DOWN STRATEGY)
* -- Implementare. Analiza complexitate
*
*/


/*

Complexitati:



-> Time Complexity:
                                  WORST         AVERAGE       
    Heapify:                     O(logn)        O(logn)
    Build-Heap Bottom Up:        O(n)           O(n)
    Build-Heap Top Down:         O(nlogn)       O(nlogn) (pe average nu e tocmai vizibil pentru ca constanta de multiplicitate  este mica)
    Heap-Sort:                   O(nlogn)       O(nlogn) 
    

-> Space Complexity:
                                  
    Heapify (recursiv):          O(logn)
    Build-Heap Bottom Up:        O(1)     --> analog heapify
    Build-Heap Top Down:         O(1)       
    Heap-Sort:                   O(1)    (implementat cu Heapify nerecursiv)    /  O(nlogn) (implementat cu Heapify recursiv)


    Analiza Time Complexity pentru Build-Heap BU
    
    -> pentru frunze          n/2    ===> 0   operatii in heapify
    -> parintii frunzelor     n/2^2  ===> 1   operatie in heapify 
    -> .............          n/2^3  ===> 2   operatii in heapify
    -----
    -> radacina               n/2^h  ===> h-1 operatii in heapify
    ______________________________________(+)
                                    O(n)

*/


#include <stdio.h>
#include "Profiler.h"

Profiler prof("heapBuild");

#define MAX_HEAP 60000
#define STEP_SIZE 100
#define NBS_TESTS 5
#define INF 50000


typedef struct
{
    int heap[MAX_HEAP];
    int lengthOfHeap;
}HeapTD;


/*           FUNCTII AUXILIARE             */


void swapping(int* a, int* b)
{
    int c = *a;
    *a = *b;
    *b = c;
}


void copyArray(int original[], int copy[], int no, int* nc)
{
    *nc = no;
    for (int i = 0; i < no; i++)
    {
        copy[i] = original[i];
    }
}

void showHeap(int heap[], int lengthOfHeap, int when)
{
    int prettyOutput = 1;
    int i = 0;
    
    if (!when)
    {
        printf("\nAfter building Heap: \n");
    }
    else if ( when == 1)
    {
        printf("\nBefore building Heap: \n");

    }
    else
    {
        printf("\nHeapSort result:\n");
    }

    int nbLevels = 1;

    while (prettyOutput  <= lengthOfHeap)
    {
        for (int j = i; j < i + prettyOutput && j < lengthOfHeap; j++)
        {
            printf("%d ", heap[j]);
        }
        i += prettyOutput;
        printf("\n");
        prettyOutput *= 2;
        nbLevels--;
    }

    printf("\n");
}


/*           BUILD-HEAP | BOTTOM UP STRATEGY             */


int getIndexOfLargest(int heap[], int index, int length, Operation* opAttr, Operation* opComp)
{   
    int indexOfLargest = index;
    
    (*opComp).count();
    if (2 * index + 1 < length && heap[indexOfLargest] < heap[2 * index + 1]) // left
    {
        indexOfLargest = 2 * index + 1;
    }

    (*opComp).count();
    if (2 * index + 2 < length && heap[indexOfLargest] < heap[2 * index + 2]) // right
    {
        indexOfLargest = 2 * index + 2;
    }

    return indexOfLargest;
}

void heapify(int heap[], int index, int length, Operation *opAttr, Operation *opComp)
{

    int largestValueIndex = getIndexOfLargest(heap, index, length, opAttr, opComp);

    if (largestValueIndex != index)
    {
        (*opAttr).count(3);
        swapping(&heap[index], &heap[largestValueIndex]);
        
        heapify(heap, largestValueIndex, length, opAttr, opComp);
    }

}

void buildHeapBottomUp(int heap[], int lengthOfHeap, const char attrName[], const char compName[])
{
    /*
    ****  Cum functioneaza algoritmul?
     
     ~ Pornim de la  la faptul ca frunzele ( ce reprezinta jumatate din nodurile heap-ului ) deja reprezinta un heap deci nu trebuie sa mai treaca prin functia de heapify
     ~ Pentru restul nodurilor ce au cel putin un fiu procedam ca si cand am lega subarborele drept de cel stang prin nodul curent , dupa care aplicam operatia de heapify

    */


    Operation opAttr = prof.createOperation(attrName, lengthOfHeap);
    Operation opComp = prof.createOperation(compName, lengthOfHeap);

    for (int i = lengthOfHeap / 2 - 1; i >= 0; i--)
    {
        heapify(heap, i,lengthOfHeap,&opAttr, &opComp);
    }
}




/*                 HEAP SORT                 */


void heapSort(int heap[], int lengthOfHeap, const char attrName[], const char compName[])
{

    /*
     - Cum functioneaza?
      Se construieste un MAX HEAP si se va inlocui ultimul element cu "radacina" heapului ( cel mai mare element ), dupa care aplicam aceeasi strategie pentru [1,n-1] , [1,n-2] , [1,n-3] ... s.a.
    */
    Operation opAttr = prof.createOperation(attrName, lengthOfHeap);
    Operation opComp = prof.createOperation(compName, lengthOfHeap);

    buildHeapBottomUp(heap, lengthOfHeap, attrName, compName);

    for (int i = lengthOfHeap - 1; i > 0; i--)
    {
        opAttr.count(3);
        swapping(&heap[0], &heap[i]);
        lengthOfHeap--;
        heapify(heap, 0, lengthOfHeap, &opAttr, &opComp);
    }
}




/*           BUILD-HEAP | TOP DOWN STRATEGY             */
void insetAndIncreseKey(int heap[], int* lengthOfHeap, int key, Operation* opAttr, Operation* opComp)
{
    int index = *lengthOfHeap;
    (*lengthOfHeap)++;
    
    heap[index] = key;

    (*opComp).count();
    if (key >= heap[index])
    {
        while (index > 0 && heap[( index - 1 ) / 2] < heap[index])   // OBSERVATIE : array-urile sunt indexate de la 0 ( spre deosebire de pseudocoduri, unde sunt indexate de la 1)
                                                                     //              deci relatia parinte-fii se transforma in:   p ( parinte )   ---> (2p+1) fiu stanga ; (2p+2) fiu dreapta
                                                                     //                                                           f ( orice fiu ) ---> (f-1)/2
        {

            (*opComp).count();
            (*opAttr).count(3);
            swapping(&heap[index], &heap[(index - 1) / 2]);
            index = (index - 1) / 2; 
        }
        (*opComp).count();
    }

}

HeapTD buildHeapTopDown(int arr[], int lengthOfArray, const char attrName[],const char compName[])
{

    /*
    
     - strategia TopDown este utila pentru datele pe care nu le cunoastem de la inceput
     - cum functioneaza ? 
     - fiecare cheie noua va fi inserata pe ultima pozitie , dupa care se va verifica daca respecta proprieteata de max-heapify prin comparatia acesteia cu parintele 
           daca nu respecta proprietatea, se va interschimba cu parintele si face aceeasi verificare pana cand propietatea e respectata sau pana cand se ajunge la radacina

    */

    Operation opAttr = prof.createOperation(attrName, lengthOfArray);
    Operation opComp = prof.createOperation(compName, lengthOfArray);

    HeapTD heap;
    heap.lengthOfHeap = 0;

    for (int i = 0; i < lengthOfArray; i++)
    {
        insetAndIncreseKey(heap.heap, &heap.lengthOfHeap, arr[i], &opAttr, &opComp);
    }
    return heap;

}


void makeItWorstForHeapBuild(int heap[], int length)
{
    /*
    
                           1
                     3           2
                 7     6      5    4 
            12    10 9    8

     - in acest fel nicio cheie nu e asezata la locul ei
     - worst case este dat de [1,3,7,9] --> 7 : 1 x heapify
                                            3 : 2 x heapify
                                            1 : 3 x heapify 
     -trebuie sa ne asiguram ca radacina( ce are valoarea minima) va ajunge in subarborele stang 
     --> de ce cel stang? nivelul incepe sa fie completat de la stanga la dreapta deci este posibil ca h(subarbore stang) > h(subarbore drept)
    
    */

    int twoPower = 1;

    while (twoPower * 2 <= length)
    {
        int j = 1;

        for (int i = twoPower - 1; i < twoPower + twoPower / 2 - 1; i++ && j++)
        {
            swapping(&heap[i], &heap[2 * twoPower - 1 - j]);
        }
       
        twoPower *= 2;
    }

    int j = 0;
    for (int i = twoPower - 1; i < (twoPower - 1 + length) / 2; i++ && j++)
    {
        swapping(&heap[i], &heap[length - j - 1]);
    }

}



enum CASES{AVERAGE, WORST};

void performance(int order, CASES caseSelection)
{
    int heap[MAX_HEAP];
    int heapCopy[MAX_HEAP];
    HeapTD heapType;

    for (int lengthHeap = STEP_SIZE; lengthHeap <= MAX_HEAP; lengthHeap += STEP_SIZE)
    {
        for (int nbTests = 0; nbTests <= NBS_TESTS; nbTests++)
        {
    
            FillRandomArray(heap, lengthHeap, 10, 50000, false, order);
            copyArray(heap, heapCopy, lengthHeap, &lengthHeap);
            
            if (caseSelection == WORST)
            {
                makeItWorstForHeapBuild(heapCopy, lengthHeap);
            }
            buildHeapBottomUp(heapCopy, lengthHeap, "attr_BU", "comp_BU");
           
            copyArray(heap, heapCopy, lengthHeap, &lengthHeap);
            heapType = buildHeapTopDown(heapCopy, lengthHeap, "attr_TD", "comp_TD");


            if (caseSelection == WORST)
            {
                makeItWorstForHeapBuild(heap, lengthHeap);
            }
            heapSort(heap, lengthHeap, "attr_HS", "comp_HS");

            
        }
    }

   
        prof.divideValues("attr_BU", NBS_TESTS);
        prof.divideValues("comp_BU", NBS_TESTS);
        prof.addSeries("BottomUp", "attr_BU", "comp_BU");       

        prof.divideValues("attr_TD", NBS_TESTS);
        prof.divideValues("comp_TD", NBS_TESTS);
        prof.addSeries("TopDown", "attr_TD", "comp_TD");

        prof.divideValues("attr_HS", NBS_TESTS);
        prof.divideValues("comp_HS", NBS_TESTS);
        prof.addSeries("total_HeapSort", "attr_HS", "comp_HS");

        prof.createGroup("BottomUp_VS_TopDown", "BottomUp", "TopDown");

}

void performance_all()

{
    prof.reset("average");
    performance(UNSORTED, AVERAGE);
    
    prof.reset("worst");
    performance(ASCENDING,WORST);
    prof.showReport();
}

void demo()
{
    int heapBU[] = { 1, 3, 5, 4, 6, 13, 10, 9, 8, 15, 17 };
    int nBU = sizeof(heapBU) / sizeof(int);

    printf("Build-Heap | BottomUp Strategy:\n");
    showHeap(heapBU, nBU, 1);
    buildHeapBottomUp(heapBU, nBU, "ThisIs", "ADemo");
    showHeap(heapBU, nBU, 0);

    int arr[] = {35, 33, 42, 10, 19, 27, 44, 26, 31};
    int n = sizeof(arr) / sizeof(int);
    printf("Build-Heap | TopDown Strategy:\n");
    showHeap(arr, n, 1);
    HeapTD heapTD = buildHeapTopDown(arr, n, "ThisIs", "ADemo");
    showHeap(heapTD.heap, heapTD.lengthOfHeap, 0);


    int heapS[] = { 1, 3, 5, 4, 6, 13, 10, 9, 8, 15, 17 };
    int nSort = sizeof(heapS) / sizeof(int);
    heapSort(heapBU, nBU, "ThisIs", "ADemo");
    showHeap(heapBU, nBU, 3);

}
int main()
{
    demo();
    //performance_all();
}