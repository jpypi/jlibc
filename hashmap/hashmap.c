/*
 * Author: James Jenkins
 * Desc: A hashmap implementation of my own construction. (Excluding the
 *       JVM hash function.) Works with strings as keys and void* for
 *       values.
 */
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"


HashMap *
initHashMap(int size, HashFunctionType(hash_function)) {
    HashMap *h = malloc(sizeof(HashMap));

    h -> size = size;
    h -> elements = calloc(size, sizeof(HashMapNode*));
    if (hash_function == NULL) {
        h -> hash_function = jvmStringHash;
    } else {
        h -> hash_function = hash_function;
    }

    return h;
}


void *
putElement(HashMap *hm, char *key, void *value) {
    HashMapNode *hn;

    int hash_pos = hm->hash_function(hm, key);

    if (hm->elements[hash_pos] == NULL) {
        hm -> length += 1;
        // Create a new hash node
        hn = malloc(sizeof(HashMapNode));
        int key_len = strlen(key);
        hn -> key = malloc(key_len + 1);
        strcpy(hn->key, key);
        hn -> key[key_len] = 0;

        hn -> value = value;
        hn -> next = NULL;

        hm->elements[hash_pos] = hn;
    } else {
        // Start at the first element in the hash bin
        hn = hm->elements[hash_pos];
        while (1) {
            // Found duplicate key. Return the previous value so the user can
            // do what they need to with it. (like maybe free it)
            if (!strcmp(hn->key, key)) {
                void *lost = hn->value;
                hn -> value = value;
                return lost;
            } else if (hn->next != NULL){
                // Just traverse the linked list
                hn = hn->next;
            } else {
                hm -> length += 1;
                // Create a new node
                HashMapNode *new_node = malloc(sizeof(HashMapNode));
                new_node -> key = malloc(strlen(key) + 1);
                strcpy(new_node->key, key);
                new_node -> value = value;
                new_node -> next = NULL;

                // Tack this node on to the end of the linked list
                hn->next = new_node;

                return NULL;
            }
        }
    }

    /*
    Pretty sure this is never used, but GCC complains.
    (stackoverflow.com/questions/15927583) suggests the following could be used:
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wreturn-type"
    // Your code here
    #pragma GCC diagnostic pop
    */
    return NULL;
}


void *
getElement(HashMap *hm, char *key) {
    // Calculate the bin this key would be in
    if (key == NULL) return NULL;
    int hash_pos = hm -> hash_function(hm, key);

    if (hm -> elements[hash_pos] != NULL) {
        // Search through the linked list at the hash bin
        HashMapNode *current_node = hm -> elements[hash_pos];
        // Loop until finding the end of the LL (didn't find key) or found
        while (current_node != NULL && strcmp(current_node->key, key))
            current_node = current_node->next;

        if (current_node != NULL)
            return current_node -> value;
    }

    return NULL;
}


void
freeHashMap(HashMap *hm, void(free_val)(void *)) {
    // Loop through hash bins
    for (int i = 0; i < hm->size; i++) {
        // Loop through linked list
        HashMapNode *current_node = hm->elements[i], *next_node;
        while (current_node != NULL) {
            next_node = current_node -> next;
            free(current_node->key);
            if (free_val != NULL)
                free_val(current_node->value);
            free(current_node);
            current_node = next_node;
        }
    }

    free(hm->elements);
    free(hm);
}


// A simple (and probably quite naive) string hashing function
unsigned int
stringHash(HashMap *hm, char *string) {
    unsigned int value = 0;
    for (unsigned int i = 0; string[i] != 0; i++)
        value += string[i];

    return value % hm->size;
}


// This would appear to be the hash function used by the JVM. I'm assuming it's
// better than my naive function.
// https://www.quora.com/What-are-some-ways-to-generate-a-hash-for-a-string
// http://stackoverflow.com/questions/2624192/good-hash-function-for-strings
unsigned int
jvmStringHash(HashMap *hm, char *string) {
    unsigned int value = 7;
    for (unsigned int i = 0; string[i] != 0; i++)
        value = value * 31 + string[i];

    return value % hm->size;
}
