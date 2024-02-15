#include <stdio.h>
#include "Profiler.h"

Profiler p("insertionSort");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NB_TESTS 50

int binarySearch(int arr[], int left, int right, int toBeFound, Operation *opComp)
{
    if (left >= right)
    {
        return left;
    }


    int middle = (left + right) / 2;

    (*opComp).count();
    if (arr[middle] <= toBeFound)
    {
        return binarySearch(arr, middle + 1, right, toBeFound, opComp);
    }
    else
    {
        return binarySearch(arr, left, middle, toBeFound, opComp);
    }

}

void insertionSortWithBinarySearch(int arr[], int n)
{

    Operation opComp = p.createOperation("binary-comp", n);
    Operation opAttr = p.createOperation("binary-attr", n);

    for (int i = 1; i < n; i++)
    {

        int insertPosition = binarySearch(arr, 0, i, arr[i], &opComp);
        
        int toBeSwapped = arr[i];

        for( int j = i; j > insertPosition; j--)
        {
            opAttr.count();
            arr[j] = arr[j - 1];
        }
        opAttr.count();
        arr[insertPosition] = toBeSwapped;
    }
}

void insertionSortClassic(int arr[], int n)
{
    Operation opComp = p.createOperation("classic-comp", n);
    Operation opAttr = p.createOperation("classic-attr", n);

    for (int i = 1; i < n; i++)
    {
        int toBeSwapped = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > toBeSwapped)
        {
            opComp.count();
            opAttr.count();
            arr[j + 1] = arr[j];
            j--;
        }
        opAttr.count();
        arr[j + 1] = toBeSwapped;
    }
}

void showArray(int arr[], int sizeArray)
{
    for (int i = 0; i < sizeArray; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}


void copyArray(int v[], int vCopy[], int n)
{
    for (int i = 0; i < n; i++)
    {
        vCopy[i] = v[i];
    }
}

void performance(int order)
{
    int v[MAX_SIZE];
    int vCopy[MAX_SIZE];
    int n;
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for (int test = 0; test < NB_TESTS; test++)
        {
            FillRandomArray(v, n, 10, 50000, false, order);
            copyArray(v, vCopy, n);
            insertionSortClassic(v, n);
            insertionSortWithBinarySearch(vCopy, n);
        }
    }

    p.divideValues("classic-attr", NB_TESTS);
    p.divideValues("classic-comp", NB_TESTS);
    p.addSeries("classic", "classic-attr", "classic-comp");

    p.divideValues("binary-attr", NB_TESTS);
    p.divideValues("binary-comp", NB_TESTS);
    p.addSeries("binary", "binary-attr", "binary-comp");

    p.createGroup("attr", "classic-attr", "binary-attr");
    p.createGroup("comp", "classic-comp", "binary-comp");
    p.createGroup("total", "classic", "binary");

}

void perf_all()
{
    performance(UNSORTED);
 
    p.showReport();
}


void demo()
{

    printf("insertion sort classic: \n");
    int testInsertSort[] = { 9, 3, 12, 5, 7, 2, 9, 5 };
    int n = sizeof(testInsertSort) / sizeof(testInsertSort[0]);

    printf("Before Sorting: ");
    showArray(testInsertSort, n);

    insertionSortClassic(testInsertSort, n);
    
    printf("After Sorting: ");
    showArray(testInsertSort, n);

    printf("insertion sort with binary search: \n");
    int testInsertSortBS[] = { 9, 3, 12, 5, 7, 2, 9, 5 };
    int nBS = sizeof(testInsertSortBS) / sizeof(testInsertSortBS[0]);

    printf("Before Sorting: ");
    showArray(testInsertSortBS, nBS);

    insertionSortWithBinarySearch(testInsertSortBS, nBS);
    
    printf("After Sorting: ");
    showArray(testInsertSortBS, nBS);


}


int main()
{
    //demo();
    perf_all();
}



/*
/// cateva explicatii din sticky notes:

daca nu gaseste da pozitia urmatorului celui mai mare
daca il gaseste da pozitia urmatorului ( eg 3 3 3 4) => poz 3 ( a lui 4)

1 3 3 3 5 9 (cauta 2 => ret 1 inseret pe 1)
1 -2- 3 3 3 5 9 ( cauta 3 => ret  5 inserez pe 3)

1 -2- 3 3 3 -3- 5


0,1, 2, 3,4,5,6,7
9, 3,12,5,7,2,9,5
3,9,12 caut 5 => poz 1 de inserat
3,5,9,12


*/