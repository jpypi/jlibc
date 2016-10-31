#ifndef __J_HASHMAP__
#define __J_HASHMAP__

#define HashFunctionType(x) unsigned int(*x)(struct __hashmap *, char *)

typedef struct __hashmap_node {
    char *key;
    void *value;
    struct __hashmap_node *next;
} HashMapNode;

typedef struct __hashmap {
    int size;
    int length;
    HashMapNode **elements;
    HashFunctionType(hash_function);
} HashMap;

// It would be wise to use a prime for size something to do with factors
// that I don't recall exactly, but the size ends up being used as the mod
// value. You'll be happier if you use a prime.
HashMap * initHashMap(int size, HashFunctionType(hash_function));

void * getElement(HashMap *hm, char *key);
void * putElement(HashMap *hm, char *key, void *value);
void * delElement(HashMap *hm, char *key);

// Will free all resources used by the hashmap and if `free_val` is not null
// the `free_val` function will be called for each value in the HashMap. (one
// could easily just pass in `free` from stdlib)
void freeHashMap(HashMap *hm, void(free_val)(void *));

unsigned int stringHash(HashMap *, char *string);
unsigned int jvmStringHash(HashMap *, char *string);

#endif
