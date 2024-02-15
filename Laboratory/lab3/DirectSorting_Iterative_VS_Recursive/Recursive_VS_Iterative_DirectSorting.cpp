/**
*@author Voicu Laura-Luisa
*@group 30226 - semigrupa 2
*
* ANALIZA COMPARATIVA VERSIUNE ITERATIVA VS RECURSIVA : SELECTION SORT, BUBBLE SORT
* -- Implementare. Analiza grafice
*
*/


#include <stdio.h>
#include "Profiler.h"
Profiler p("recursiveIterative");
#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NBS_TESTS 5

enum {AVERAGE,WORST,BEST};
void swapping(int* a, int* b)
{
    int c = *a;
    *a = *b;
    *b = c;
}



void showArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void copyArray(int arr[],int arrCopy[], int n)
{
    for (int i = 0; i < n; i++)
    {
        arrCopy[i] = arr[i];
    }
}


void makeItWorstForSelection(int v[], int n)
{
    // tin cont ca v e descrescator
    int auxArray[MAX_SIZE];

    auxArray[0] = v[0];

    for (int i = 1; i < n; i++)
    {
        auxArray[i] = v[n - i];
    }
    copyArray(auxArray, v, n);
}



/*           RESURSIVE SELECTION SORT           */

int getMinFromArrayRecursive(int arr[], int left, int right, Operation* opSelection)
{
    if (left == right)
    {
        return left;
    }

    int minPosition = getMinFromArrayRecursive(arr, left + 1, right, opSelection);

    (*opSelection).count();
    if (arr[left] < arr[minPosition])
    {
        return left;
    }

    return minPosition;
}

void selectionSortRecursive(int arr[], int length, int currentIndex, Operation *opSelection)
{
    if (currentIndex < length - 1)
    {
        int minIndex = getMinFromArrayRecursive(arr, currentIndex , length - 1 , opSelection);

        if (minIndex != currentIndex)
        {
            (*opSelection).count(3);
            swapping(&arr[currentIndex], &arr[minIndex]);
        }
        selectionSortRecursive(arr, length, currentIndex + 1, opSelection);
    }
}


/*                   ITERATIVE SELECTION SORT                     */   
void selectionSort(int arr[], int n, Operation* opSelection)
{

    for (int i = 0; i < n - 1; i++)
    {
        int minPosition = i; 

        for (int j = i + 1; j < n; j++)
        {
           
            (*opSelection).count();
            if (arr[j] < arr[minPosition])
            {
                minPosition = j;
            }
        }

        if (minPosition != i)
        {
            (*opSelection).count(3);
            swapping(&arr[i], &arr[minPosition]);
        }
    }
}

/*                  BUBBLE SORT                 */    

/*          RECURSIVE BUBBLE SORT               */
void bubbleSortRecursive(int arr[], int length, int i, int j, int* stillUnsorted, Operation* op)
{
    if (*stillUnsorted == 0)
    {
        return;
    }

    if (i < length - 1)
    {
 
        if (j < length - i - 1)
        {
            (*op).count();
            if (arr[j] > arr[j + 1])
            {
                (*op).count(3);
                swapping(&arr[j], &arr[j + 1]);
                *stillUnsorted = 1;
            }
            bubbleSortRecursive(arr, length, i, j + 1, stillUnsorted, op);
        }
        else
        {
            bubbleSortRecursive(arr, length, i + 1, 0, stillUnsorted, op);
        }
    }

}
/*                  ITERATIVE  BUBBLESORT                    */        
void bubbleSort(int arr[], int n, Operation *op)
{


    int stillUnsorted;
    int lengthArray = n;

    do
    {
        stillUnsorted = 0;
        for (int i = 0; i < lengthArray - 1; i++)
        {
            (*op).count();
            if (arr[i] > arr[i + 1])
            {
                (*op).count(3);
                swapping(&arr[i], &arr[i + 1]);
                stillUnsorted = 1;
            }
        }

        lengthArray--;
    } while (stillUnsorted);
}


void performance(int order, int selectedCase)
{

    int* arr = (int*)malloc(sizeof(int) * MAX_SIZE);
    int* arrCopy = (int*)malloc(sizeof(int) * MAX_SIZE);
    int** matrixCopy = (int**)malloc(sizeof(int*) * NBS_TESTS); // copiez fiecare
    for (int i = 0; i < NBS_TESTS; i++)
    {
        matrixCopy[i] = (int*)malloc(sizeof(int) * MAX_SIZE);
    }


    for (int length = STEP_SIZE; length <= MAX_SIZE; length += STEP_SIZE)
    {

        Operation opSelection = p.createOperation("selectionSort", length);
        Operation opSelectionRec = p.createOperation("selectionSortRecursive", length);

        Operation opBubble = p.createOperation("bubbleSort", length);
        Operation opBubbleRec = p.createOperation("bubbleSortRecursive", length);

        for (int nbTests = 0; nbTests < NBS_TESTS; nbTests++)
        {
            FillRandomArray(arr, length, 10, 50000, false, order);
            copyArray(arr, matrixCopy[nbTests], length); // am copiat in matrixCopy continutul
            copyArray(arr, arrCopy, length);

            int unsorted = 1;
            bubbleSort(arr, length, &opBubble);
            bubbleSortRecursive(arrCopy, length, 0, 0, &unsorted, &opBubbleRec);

            copyArray(matrixCopy[nbTests], arr, length);
            copyArray(arr, arrCopy, length);
            if (selectedCase == WORST)
            {
                makeItWorstForSelection(arr, length);
            }
            selectionSort(arr, length, &opSelection);
            selectionSortRecursive(arrCopy, length, 0, &opSelectionRec);


            
        }

        // SELECTION SORT
        p.startTimer("selectionSort", length);
        for (int nbTests = 0; nbTests < NBS_TESTS; nbTests++)
        {
            copyArray(matrixCopy[nbTests], arr, length);
            selectionSort(arr, length, &opSelection);
        }
        p.stopTimer("selectionSort", length);

        p.startTimer("selectionSortRecursive", length);
        for (int nbTests = 0; nbTests < NBS_TESTS; nbTests++)
        {
            copyArray(matrixCopy[nbTests], arr, length);
            selectionSortRecursive(arr, length, 0, &opSelectionRec);
        }
        p.stopTimer("selectionSortRecursive", length);

        // BUBBLE SORT

        p.startTimer("bubbleSort", length);
        for (int nbTests = 0; nbTests < NBS_TESTS; nbTests++)
        {
            copyArray(matrixCopy[nbTests], arr, length);
            bubbleSort(arr, length, &opBubble);
        }
        p.stopTimer("bubbleSort", length);
       
        p.startTimer("bubbleSortRecursive", length);
        for (int nbTests = 0; nbTests < NBS_TESTS; nbTests++)
        {
            int unsorted = 1;
            copyArray(matrixCopy[nbTests], arr, length);
            bubbleSortRecursive(arr, length, 0, 0, &unsorted, &opBubble);
        }
        p.stopTimer("bubbleSortRecursive", length);


    }
    p.createGroup("Selection Sort", "selectionSort", "selectionSortRecursive");
    p.createGroup("Bubble Sort", "bubbleSort", "bubbleSortRecursive");


 
}

void demo()
{
    int arr[] = { 3,-12,45,3,90,14,26,0 };
    int n = sizeof(arr) / sizeof(int);
    Operation op = p.createOperation("hei", n);
    
    printf("\n_______ Selection Sort Recursive ________\n");
    printf("Before : ");
    showArray(arr, n);
    selectionSortRecursive(arr, n, 0, &op);
    printf("After : ");
    showArray(arr, n);  

    int arrIt[] = { 3,-12,45,3,90,14,26,0 };
    int nIt = sizeof(arrIt) / sizeof(int);
    printf("\n_______ Selection Sort Iterative _______\n");
    printf("Before : ");
    showArray(arrIt, nIt);
    selectionSort(arrIt, nIt, &op);
    printf("After : ");
    showArray(arrIt, n);


    printf("\n______________________________________\n");
    int arrB[] = { 3,-12,45,3,90,14,26,0 };
    int nB = sizeof(arrB) / sizeof(int);
    int unsorted = 1;
    Operation opB = p.createOperation("hello", n);
    printf("\n_______ Bubble Sort Recursive ________\n");
    printf("Before : ");
    showArray(arrB, nB);
    bubbleSortRecursive(arrB, nB, 0, 0, &unsorted, &opB);
    printf("After : ");
    showArray(arrB, nB);


    int arrBIt[] = { 3,-12,45,3,90,14,26,0 };
    int nBIt = sizeof(arrBIt) / sizeof(int);
    printf("\n______ Bubble Sort Iterative _________\n");
    printf("Before : ");
    showArray(arrBIt, nBIt);
    bubbleSort(arrBIt, nBIt, &opB);
    printf("After : ");
    showArray(arrBIt, nBIt);

}

void performance_all()
{
    p.reset("average");
    performance(UNSORTED, AVERAGE);

    p.reset("best");
    performance(ASCENDING, BEST);

    p.reset("worst");
    performance(DESCENDING, WORST);
    p.showReport();
}


int main()
{

    demo();
    //performance_all();
}