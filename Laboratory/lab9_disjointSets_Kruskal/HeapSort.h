#pragma once

#include "Edges.h"


void swapping(int* a, int* b);
int getIndexOfLargest(Edges heap[], int index, int length);
void heapify(Edges heap[], int index, int length);
void buildHeapBottomUp(Edges heap[], int lengthOfHeap);
void heapSort(Edges heap[], int lengthOfHeap);
