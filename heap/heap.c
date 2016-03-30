#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "heap.h"

//#define DEBUG
#ifdef DEBUG
    #include "ioutils.h"
#endif


/*
    INITIALIZERS
*/

HeapNode *
newHeapNode(int *value) {
    HeapNode *new_node = malloc(sizeof(HeapNode));

    new_node->value = value;
    new_node->parent = NULL;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}


Heap*
newHeap() {
    Heap *heap = malloc(sizeof(Heap));
    heap->top = NULL;
    heap->size = 0;

    return heap;
}


// 1 = right; 0 = left
HeapNode*
traverse(HeapNode *node, char direction) {
    if (node != NULL) {
        if (direction == 1 && node->right != NULL)
            return node->right;
        if (direction == 0 && node->left != NULL)
            return node->left;
    }

    return NULL;
}


// Use offset=0 for delete
// use offset=1 for insert
HeapNode*
findLast(Heap *heap, char ignore_last_levels) {
    // Found how to do this from:
    // stackoverflow.com/questions/19720438/pointer-based-binary-heap-implementation

    // Cases where there is no point in continuing (i.e. this is err checks)
    if (heap->top == NULL || heap->size == 0) return NULL;

    // Optimization
    if (heap->size == 1) return heap->top;

    int trav_number = heap->size;

    // -1 to ignore the first bit. -1 so we know how many bits over we need
    // to shift to find the correct bit for determining movement direction
    // __builtin_clz function found via:
    //stackoverflow.com/questions/2589096/find-most-significant-bit-left-most-that-is-set-in-a-bit-array
    // may need to use the function the 2nd answer provided
    int Nimportant_bits = sizeof(int) * 8 - __builtin_clz(trav_number) - 1 - 1;

    if (Nimportant_bits >= 0) {
        unsigned int compare_bit = 1 << Nimportant_bits;

        // Select node to start from when traversing
        HeapNode *target = heap->top;
        #ifdef DEBUG
        writeLn("Traversing...");
        writeInt(*target->value);
        writeOut("\n");
        #endif

        for (;Nimportant_bits >= ignore_last_levels; Nimportant_bits--) {
            target = traverse(target,
                             (trav_number & compare_bit) == compare_bit ? 1:0);
            #ifdef DEBUG
            writeInt(*target->value);
            writeOut("\n");
            #endif
            // Shift comparison point over 1 bit
            compare_bit >>= 1;
        }

        return target;
    }

    return NULL;
}


// This swap only works where A is the upper and B is the lower
void
swapNodes(Heap *heap, HeapNode *nodeA, HeapNode *nodeB) {
    HeapNode *temp;
    if (nodeA == 0 || nodeB == 0) return;

    nodeB->parent = nodeA->parent;

    // Only need to do these things if node A has a parent
    if (nodeA->parent != 0) {
        if (nodeA == nodeA->parent->left)
            nodeA->parent->left = nodeB;
        else
            nodeA->parent->right = nodeB;
    } else
        heap->top = nodeB;

    nodeA->parent = nodeB;

    // nodeB is nodeA's left child
    if (nodeB == nodeA->left) {
        #ifdef DEBUG
        printf("Node B is left-child of A\n");
        #endif
        temp = nodeA->right;

        nodeA->right = nodeB->right;
        if (nodeA->right != 0)
            nodeA->right->parent = nodeA;

        nodeB->right = temp;
        if (nodeB->right != 0)
            nodeB->right->parent = nodeB;

        nodeA->left = nodeB->left;
        if (nodeA->left != 0)
            nodeA->left->parent = nodeA;
        nodeB->left = nodeA;

    } else {
        #ifdef DEBUG
        printf("Node B is right-child of A\n");
        #endif
        temp = nodeA->left;

        nodeA->left = nodeB->left;
        if (nodeA->left != 0)
            nodeA->left->parent = nodeA;

        nodeB->left = temp;
        if (nodeB->left != 0)
            nodeB->left->parent = nodeB;

        nodeA->right = nodeB->right;
        if (nodeA->right != 0)
            nodeA->right->parent = nodeA;
        nodeB->right = nodeA;
    }

}


void
heapWalkup(Heap *heap, HeapNode *node) {
    if (node->parent == NULL) {
        #ifdef DEBUG
        printf("NULL parent\n");
        #endif
        return;
    }

    if (*node->value < *node->parent->value) {
        #ifdef DEBUG
        printf("Cmp: %d < %d \n", *node->value, *node->parent->value);
        #endif
        swapNodes(heap, node->parent, node);
        // Keep trying to walk the node up to the top if it has a parent
        if (node->parent != NULL)
            heapWalkup(heap, node);

    }

}


void
heapWalkdown(Heap *heap, HeapNode *node) {
    HeapNode *temp;

    if (*node->left->value < *node->value) {
        if (node->left->value < node->right->value || node->right == NULL)
            swapNodes(heap, node, node->left);
        else {
            temp = node->left;
            swapNodes(heap, node, node->left);
            swapNodes(heap, temp, temp->right);
        }

    } else if (node->right != NULL && *node->right->value < *node->value) {
        if (node->right->value < node->left->value || node->left == NULL) {
            swapNodes(heap, node, node->right);
        } else {
            temp = node->right;
            swapNodes(heap, node, node->right);
            swapNodes(heap, temp, temp->left);
        }
    }
}


void
heapInsert(Heap *heap, int *value) {
    HeapNode *new_node = newHeapNode(value);
    heap->size += 1;

    if (heap->top == NULL) {
        heap->top = new_node;
    } else {
        #ifdef DEBUG
        printf("Doing walkup insert\n");
        #endif

        HeapNode *last_node = findLast(heap, 1);
        new_node->parent = last_node;

        if (new_node->parent->left == 0) {
            #ifdef DEBUG
            printf("Setting to left child.\n");
            #endif
            new_node->parent->left = new_node;
        } else {
            #ifdef DEBUG
            printf("Setting to right child.\n");
            #endif
            last_node->right = new_node;
        }

        #ifdef DEBUG
        printf("new_node->parent value: %d\n", *new_node->parent->value);
        #endif

        heapWalkup(heap, new_node);
    }
}


HeapNode *
heapPop(Heap *heap) {
    if (heap->size == 0) return NULL;

    HeapNode *to_return = heap->top;

    if (heap->size > 1) {
        HeapNode *last_node = findLast(heap, 0);
        // Clean up last_node's parent's ref to it
        if (last_node->parent->left == last_node)
            last_node->parent->left = NULL;
        if (last_node->parent->right == last_node)
            last_node->parent->right = NULL;

        // Take the last and make it the new top
        heap->top = last_node;
        heap->top->parent = NULL; // New top has no parent

        // Set the children of the new top
        heap->top->left  = to_return->left;
        heap->top->right = to_return->right;

        heapWalkdown(heap, heap->top);
    }

    heap->size -= 1;

    return to_return;
}
