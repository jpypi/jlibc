#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "vector.h"


void
addElement(Vector *vec, void *element){
    vec->elements[vec->length] = element;

    vec->length += 1;

    // If the new length is greater than the capacity then realloc the elements
    // array
    if (vec->length >= vec->capacity) {
        // Compute a new capacity to grow to
        vec->capacity = floor(vec->capacity * vec->growth_rate);
        vec->elements = realloc(vec->elements, sizeof(void*) * vec->capacity);
    }
}


// Remove an arbitrary element from the vector and shift all the elements after
// it so that there are no gaps.
// Returns the element if element < vec->length and vec->length > 0 otherwise
// returns NULL
void *
popElement(Vector *vec, int element) {
    if (vec->length > 0) {
        for (int i = 0; i < vec->length; i++) {
            if (i == element) {
                vec->length--;
                int num_elements = vec->length - element;

                void *copy = vec->elements[i];
                if (vec->length > 0) {
                    memmove(&vec->elements[i], &vec->elements[i+1],
                            sizeof(void*) * num_elements);
                }

                return copy;
            }
        }
    }

    return NULL;
}


// Call free on all the elements in the vector then reset the length to 0, but
// keeps the space for the elements (the elements array) allocated for future
// use. Capacity also remains the same
void
freeElmsEmptyVector(Vector *vec) {
    // Call free on all the elements in the vector
    for (int i = 0; i < vec->length; i++) {
        free(vec->elements[i]);
    }
    // Set the vector's length to 0 because it's now empty.
    vec->length = 0;
}


// Call free on all the elements in the vector then free the element array
// then free the vector
void
freeVector(Vector *vec) {
    for (int i = 0; i < vec->length; i++) {
        free(vec->elements[i]);
    }

    free(vec->elements);
    free(vec);
}


// Mallocs space for the vector struct and mallocs space to initialize the
// elements array
Vector *
initVector(int initial_capacity, float growth_rate) {
    Vector *v = malloc(sizeof(Vector));

    v->length = 0;
    v->capacity = initial_capacity;
    v->elements = malloc(sizeof(void*) * initial_capacity);

    // Provide a default growth rate (0 would be the same as a regular array)
    if (growth_rate == 0)
        v->growth_rate = 1.5;
    else
        v->growth_rate = growth_rate;

    return v;
}
