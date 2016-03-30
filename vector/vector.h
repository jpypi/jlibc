#ifndef VECTOR_H
#define VECTOR_H

typedef struct VectorS {
    void **elements;
    int length;
    int capacity;
    float growth_rate;
} Vector;

void addElement(Vector *, void *);

// Returns -1 if Vector is empty
void *popElement(Vector *, int);

// Pass in initial size and growth rate
// growth rate, if 0 is 1.5
Vector *initVector(int, float);

// All elements in vector MUST pointers on which free() can be called!
// Completely cleans up vector's elements, vector's space, and vector control
// struct
void freeVector(Vector *);

// Call free() on all elements in vector
void freeElmsEmptyVector(Vector *);

#endif
