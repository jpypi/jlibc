#ifndef HEAP_H
#define HEAP_H

typedef struct Node {
    int * value;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} HeapNode;

typedef struct HeapStruct {
    HeapNode *top;
    int size;
} Heap;

// For creating a new heap
Heap * newHeap();

// To add items to the heap
void heapInsert(Heap *, int *);

// To get an item from the heap
HeapNode * heapPop(Heap *);

// the char value is how many levels up from last you'd like to go
HeapNode * findLast(Heap *, char);

#endif
