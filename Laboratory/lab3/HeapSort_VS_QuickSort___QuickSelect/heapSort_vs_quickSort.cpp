/**
*@author Voicu Laura-Luisa
*@group 30226 - semigrupa 2
*
* HeapSort VS QuickSort . Quick Selection. Partition Algorithms
* -- Implementare. Analiza complexitati. Comparatie QuickSort vs HeapSort
*
*/

/*
*        Preview :)
* 
*        ~ QuickSort
*   o Running Time:
*   Worst  : O(n²) (not optimized) (are loc pentru sir sortat crescator/descrescator si pivotul = first/last)   
           / O(nlog) (optimized -> randomized Partition (e aproape sigur ca nu se va lua primul/ultimul element din sirul sortat) \ Median selection with AKL's algorithm pentru a >= 5) 
*   Average: O(nlogn)
*   Best   : O(nlogn) ~(are loc atunci cand pivotul este chiar mijlocul sirului sortat)
*   
*   o Space Complexity:
*   Worst : O(log n) --> suficient de putin cat sa consideram algoritmul optimal (tinand cont si de optimizarile pentru partitii)
*   
*        ~ QuickSort VS HeapSort
*   R.T. pentru HeapSort: W = B = A = O(nlog n)
*   Space Complexity HS : O(1)    
*   ===> HeapSort este optimal
*   Totusi, in practica QuickSort optimizat este mai rapid
* 
*       ~ QuickSelect(Randomized-Select)
*    o Running Time
*     Worst     : O(n²) (not optimized) - O(n) cu optimizarile de la QuickSort
*     Average   : O(n)
*     Average   : O(n)
* 
*       ~ Input arrangements
*     QuickSort :
*     o Worst: Sir sortat crescator + pivotul e ultimul element folosesc Lomuto partition 
*     o Best : Sir sortat crescator + pivotul e chiar mijlocul elementului -> foloses Hoare's partition updated ( nu foloses varianta originala pentru ca acolo apelurile recursive sunt de tipul QS(p,q) si QS(q+1,r) , pe cand la Lomuto si Hoare Updated sunt QS(p,q-1) si QS(q,r), dar ar functiona la fel de bine 
*                                                                          -> Obs: nu e necesar sa folosesc AKL pentru ca STIU ca sirul e ORDONAT deci stiu unde e elementul din mijloc MEREU (+ e mai greu de implementat :D )
*/


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>     
#include <time.h> 
#include <string.h>
#include "Profiler.h"
#define MAX_DIM 10000
#define STEP_SIZE 100
#define NBS_TESTS 5

Profiler prof("heapBuild");

enum Cases{AVERAGE, WORST, BEST};


/*                  FUNCTII AUXILIARE                     */

void swapping(int* a, int* b)
{
    int c = *b;
    *b = *a;
    *a = c;
}

void showArray(int arr[], int start, int stop)
{
    for (int i = start; i <= stop; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void copyArray(int original[], int copy[], int no)
{
    for (int i = 0; i < no; i++)
    {
        copy[i] = original[i];
    }
}




  /************************************************/
 /*                   PARTITIONS                 */
/************************************************/

/*
* Cum functioneaza?
* Se alege pivotul ( dupa cazuri ) dupa care sirul se imparte in 2 subsiruri (lungimile variaza in functie de pozitia pivotului!!!) :
*                           -> in stanga pivotului se alfa elementele <=
*                           -> in dreapta pivotului se alfa elementele >=
* Se vor face astfel de partitii (Divide) pana cand se ajunge la un subsir de 1 element ( care e ordonat ) si se combina subsirurile (Comine) pana se ajunge la cel initial( DAR sortat) ==> de partea asta se ocupa QuickSort :)
*/

int HoarePartitionUpdated(int arr[], int left, int right, Operation* operation)
{
    int x = arr[(left + right) / 2];
    int i = left;
    int j = right;
    
    do
    {
        (*operation).count();
        while (arr[i] < x)
        {
            i++;
        }
        (*operation).count();
        while (arr[j] > x)
        {
            j--;
        }
        if (i <= j)
        {
            (*operation).count(3);
            swapping(&arr[i], &arr[j]);
            i++;
            j--;
        }

    } while (i < j);

    return i;
}
int HoarePartitionOriginal(int arr[], int left, int right, Operation* operation)
{
    int x = arr[left];
    int i = left - 1;
    int j = right + 1;

    while (i <= j)
    {
        do
        {
            (*operation).count();
            j--;
        } while (arr[j] > x);
        do
        {
            (*operation).count();
            i++;
        } while (arr[i] < x);

        if (i < j)
        {
            (*operation).count();
            swapping(&arr[i], &arr[j]);
        }
        else
        {
            return j;
        }
    }
} 
int partitionLomuto(int arr[], int left, int right, Operation* operation)
{
    (*operation).count();
    int pivot = arr[right];
    int index = left - 1;
    for (int i = left; i < right; i++)
    {
        (*operation).count();
        if (arr[i] <= pivot)
        {
            index++;
            (*operation).count(3);
            swapping(&arr[i], &arr[index]);
        }
    }
    (*operation).count(3);
    swapping(&arr[index + 1], &arr[right]);
    return index + 1;
}


/*          CAZURI PARTITIONARE          */
int randomizedPartition(int arr[], int left, int right, Operation* operation)
{
    // average case for QuickSort

    int i = rand() % (right - left + 1) + left;

    swapping(&arr[i], &arr[left]);

    return partitionLomuto(arr, left, right, operation);
   
}

int bestCasePartition(int arr[], int left, int right, Operation* operation)
{
    return HoarePartitionUpdated(arr, left, right, operation);

}

int worstCasePartition(int arr[], int left, int right, Operation* operation)
{
    // stiu ca sirul e sortat --> voi lua ca pivot ultimul element --> nu interschimb nimic
    return partitionLomuto(arr, left, right, operation);

}


   /************************************************/
  /*                    SORTING                   */        
 /*                   QUICK SORT                 */  
/************************************************/

void quickSort(int arr[], int left, int right, Cases selectedCase, Operation* operation)
{
    if (left < right)
    {
        int pivot;
        if (selectedCase == AVERAGE)
        {
            pivot = randomizedPartition(arr, left, right, operation);
        }
        else if (selectedCase == WORST)
        {
            pivot = worstCasePartition(arr, left, right, operation);
        }
        else if (selectedCase == BEST)
        {
            pivot = bestCasePartition(arr, left, right, operation);
        }
        quickSort(arr, left, pivot - 1, selectedCase, operation);
        quickSort(arr, pivot, right, selectedCase, operation);

    }
}

void quickSortInit(int arr[], int length, Cases selectedCase)
{
    Operation operation = prof.createOperation("operations_QS", length);
    quickSort(arr, 0, length - 1, selectedCase, &operation);
}



/*                  HEAP SORT                   */

int getIndexOfLargest(int heap[], int index, int length, Operation* operation)
{
    int indexOfLargest = index;

    (*operation).count();
    if (2 * index + 1 < length && heap[indexOfLargest] < heap[2 * index + 1]) // left
    {
        indexOfLargest = 2 * index + 1;
    }

    (*operation).count();
    if (2 * index + 2 < length && heap[indexOfLargest] < heap[2 * index + 2]) // right
    {
        indexOfLargest = 2 * index + 2;
    }

    return indexOfLargest;
}

void heapify(int heap[], int index, int length, Operation* operation)
{

    int largestValueIndex = getIndexOfLargest(heap, index, length, operation);

    if (largestValueIndex != index)
    {
        (*operation).count(3);
        swapping(&heap[index], &heap[largestValueIndex]);

        heapify(heap, largestValueIndex, length, operation);
    }

}

void buildHeapBottomUp(int heap[], int lengthOfHeap, Operation* operation)
{
    for (int i = lengthOfHeap / 2 - 1; i >= 0; i--)
    {
        heapify(heap, i, lengthOfHeap, operation);
    }
}

void heapSort(int heap[], int lengthOfHeap)
{

    Operation operation = prof.createOperation("operations_HS", lengthOfHeap);


    buildHeapBottomUp(heap, lengthOfHeap, &operation);

    for (int i = lengthOfHeap - 1; i > 0; i--)
    {
        operation.count(3);
        swapping(&heap[0], &heap[i]);
        lengthOfHeap--;
        heapify(heap, 0, lengthOfHeap, &operation);
    }
}




  /************************************************/
 /*                 QuickSelect                  */
/************************************************/
/*
* 
* Cum Functioneaza?
* Ce il deosebeste de QuickSort? 
*        --> nu se urmareste sortarea (completa) a sirului, ci doar se rearanjeaza doar elementele de care se apropie indicele pana cand elementul de pe pozitia i ( din sirul sortat )  e gasit
* De ce are complexitate mai mica decat QuickSort ? 
*        --> Spre deosebire de QuickSort, Quick(Randomized) Selection face doar un apel recursiv , deci T(n) = 1 x T(n/b) + O(n)  - ( b variaza in functie de partition ) -> randomizedPartition (in majoritatea cazurilor) asigura complexitate liniara pentru partition => complexitate QSelect = O(n)
* 
*/

int quickSelection(int arr[], int left, int right, int desiredRank, Operation* operation) // = randomizedSelection
{
    if (left == right)
    {
        return arr[left];
    }
     
    int pivot = randomizedPartition(arr, left, right, operation);
    int lengthLeftPartition = pivot - left  + 1;
               
    if (desiredRank == lengthLeftPartition)
    {
        return arr[pivot];
    }
    else if (desiredRank < lengthLeftPartition)
    {
        return quickSelection(arr, left, pivot - 1 , desiredRank, operation);
    }
    else
    {
        return quickSelection(arr, pivot + 1, right, desiredRank - lengthLeftPartition, operation);
    }

}


int quickSelectionInit(int arr[], int length, int desiredRank)
{
    Operation operation = prof.createOperation("operations_QSel", length);
    int result = quickSelection(arr, 0, length - 1, desiredRank , &operation);
    return result;
}



void demo()
{

    
    //  QUICK SORT TESTING


    printf("\n_____________________________________________\n");
    printf("\n_________________QUICK SORT__________________\n");
    //int arrQuickSort[] = { -1242, -12, 1, 5, 12, 45, 56, 90 };
    int arrQuickSort[] = {5, 1, -12, 45, 12, 90, -1242, 56};
    int nQS = sizeof(arrQuickSort) / sizeof(int);
    printf("Before QuickSort: ");
    showArray(arrQuickSort, 0, nQS-1);
    quickSortInit(arrQuickSort, nQS, AVERAGE);
    printf("After QuickSort: ");
    showArray(arrQuickSort,0, nQS-1);
   

    // HEAP SORT TESTING
    printf("\n_____________________________________________\n");
    printf("\n_________________HEAP SORT___________________\n");
    int arrHeapSort[] = { 5, 1, -12, 45, 12, 90, -1242, 56 };
    int nhs = sizeof(arrHeapSort) / sizeof(int);
    printf("---Before HeapSort: ");
    showArray(arrHeapSort, 0, nhs - 1);
    quickSortInit(arrHeapSort, nhs, AVERAGE);
    printf("---After HeapSort: ");
    showArray(arrHeapSort, 0, nhs - 1);
    
    //QUICK SELECTION TESTING
    printf("\n_____________________________________________\n");
    printf("\n______________QUICK SELECTION________________\n");
    int arrQuickSelection[] = { 5, 1, -12, 45, 12, 90, -1242, 56 };
    int length = sizeof(arrQuickSelection) / sizeof(int);
    int desiredRank;
    int stillTesting;

    printf("\nInput array: ");
    showArray(arrQuickSelection, 0, length - 1);

    do {

        printf("Add desired rank : ");
        scanf("%d", &desiredRank);

        if (desiredRank <= length && desiredRank > 0)
        {
            int result = quickSelectionInit(arrQuickSelection, length, desiredRank);
            printf("%d\n", result);
        }
        else
        {
            printf("Ati introdus un rank necorespunzator!\n");
        }
        printf("Doriti sa continuati testarea pentru Quick Selection? 1/0\n");
        scanf("%d", &stillTesting);
    } while (stillTesting);


}


void performance(int order, Cases selectedCase)
{
    int* arr = (int*)malloc(MAX_DIM * sizeof(int));
    int* arrCopy = (int*)malloc(MAX_DIM * sizeof(int));
    for (int length = STEP_SIZE; length <= MAX_DIM; length += STEP_SIZE)
    {
        for (int nbTests = 0; nbTests < NBS_TESTS; nbTests++)
        {
            FillRandomArray(arr, length, 10, 5000, false, order);
            
            copyArray(arr, arrCopy, length);
            quickSortInit(arrCopy, length, selectedCase);

            copyArray(arr, arrCopy, length);
            if (selectedCase == AVERAGE)
            {
                heapSort(arrCopy, length);
                copyArray(arr, arrCopy, length); 
                quickSelectionInit(arr, length, rand() % length + 1);
            }


        }
    }

    prof.divideValues("operations_QS", NBS_TESTS);

    if (selectedCase == AVERAGE)
    {
        prof.divideValues("operations_HS", NBS_TESTS);
        prof.createGroup("HeapSort_VS_QuickSort", "operations_HS", "operations_QS");

        prof.divideValues("operations_QSel", NBS_TESTS);
        prof.createGroup("Quick_Selection", "operations_QSel");
    }
    else
    {
        prof.createGroup("Quick_Sort", "operations_QS");

    }

}

void performance_all()
{
    prof.reset("average");
    performance(UNSORTED, AVERAGE);
    prof.reset("best");
    performance(ASCENDING, BEST);
    prof.reset("worst");
    performance(ASCENDING, WORST);

    prof.showReport();
}

int main()
{
    srand(time(NULL));
    //performance_all();
    demo();
}