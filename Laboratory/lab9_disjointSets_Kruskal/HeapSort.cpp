#include "HeapSort.h"

void swapping(int* a, int* b)
{
    int c = *b;
    *b = *a;
    *a = c;
}

int getIndexOfLargest(Edges heap[], int index, int length)
{
    int indexOfLargest = index;

    if (2 * index + 1 < length && heap[indexOfLargest].w < heap[2 * index + 1].w) // left
    {
        indexOfLargest = 2 * index + 1;
    }

    if (2 * index + 2 < length && heap[indexOfLargest].w < heap[2 * index + 2].w) // right
    {
        indexOfLargest = 2 * index + 2;
    }

    return indexOfLargest;
}

void heapify(Edges heap[], int index, int length)
{

    int largestValueIndex = getIndexOfLargest(heap, index, length);

    if (largestValueIndex != index)
    {
        swapping(&heap[index].w, &heap[largestValueIndex].w);

        heapify(heap, largestValueIndex, length);
    }

}

void buildHeapBottomUp(Edges heap[], int lengthOfHeap)
{
    for (int i = lengthOfHeap / 2 - 1; i >= 0; i--)
    {
        heapify(heap, i, lengthOfHeap);
    }
}

void heapSort(Edges heap[], int lengthOfHeap)
{


    buildHeapBottomUp(heap, lengthOfHeap);

    for (int i = lengthOfHeap - 1; i > 0; i--)
    {
        swapping(&heap[0].w, &heap[i].w);
        lengthOfHeap--;
        heapify(heap, 0, lengthOfHeap);
    }
}


