#include "QuickSort.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
void swapping(Edges* a, Edges* b)
{
    Edges c = *b;
    *b = *a;
    *a = c;
}

int HoarePartitionUpdated(Edges* arr, int left, int right)
{
    Edges x = arr[(left + right) / 2];
    int i = left;
    int j = right;

    do {
        while (arr[i].w < x.w) {
            i++;
        }

        while (arr[j].w > x.w) {
            j--;
        }

        if (i <= j)
        {
            swapping(&arr[i], &arr[j]);
            i++;
            j--;
        }
    } while (i <= j);

    return i;
}

int randomizedPartition(Edges* arr, int left, int right)
{
    srand(time(NULL));
    int i = rand() % (right - left + 1) + left;
    int mid = (left + right) / 2;
    swapping(&arr[i], &arr[mid]);
    return HoarePartitionUpdated(arr, left, right);

}


void classicInsertionSort(Edges* arr, int left, int right)
{
    for (int i = left; i <= right; i++) {
        Edges rememberValue = arr[i];
        int j;
        for (j = i - 1; j >= 0 && arr[j].w > rememberValue.w; j--) {
            arr[j + 1] = arr[j];
        }
        if (j + 1 != i) { arr[j + 1] = rememberValue; }
    }
}

void quickSortHybridization(Edges* arr, int left, int right)
{
    if (left < right)
    {
        if (right < 10) {
            classicInsertionSort(arr, left, right);
        }
        else {
            int pivot = randomizedPartition(arr, left, right);
            quickSortHybridization(arr, left, pivot - 1);
            quickSortHybridization(arr, pivot, right);
        }
    }
}
