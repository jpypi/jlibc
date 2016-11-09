#include <stdio.h>
#include "hashmap.h"

int main(int argc, char *argv[]) {
    HashMap *hm = initHashMap(2, NULL);
    char *doot1 = "DOOT1";
    char *doot2 = "DOOT2";
    char *doot3 = "DOOT3";
    void *val = NULL;

    putElement(hm, "d", doot1);
    putElement(hm, "e", doot2);
    putElement(hm, "f", doot3);

    printf("hm length: %d\n", hm->length);

    val = getElement(hm, "d");
    printf("d: %s\n", (char*)val);
    val = getElement(hm, "e");
    printf("e: %s\n", (char*)val);
    val = getElement(hm, "f");
    printf("f: %s\n", (char*)val);
    printf("hm length: %d\n", hm->length);

    delElement(hm, "d");


    val = getElement(hm, "d");
    printf("d: %s\n", (char*)val);
    val = getElement(hm, "e");
    printf("e: %s\n", (char*)val);
    val = getElement(hm, "f");
    printf("f: %s\n", (char*)val);
    printf("hm length: %d\n", hm->length);

    printf("Res: %s\n", (char*)delElement(hm, "f"));
    delElement(hm, "f");


    val = getElement(hm, "d");
    printf("d: %s\n", (char*)val);
    val = getElement(hm, "e");
    printf("e: %s\n", (char*)val);
    val = getElement(hm, "f");
    printf("f: %s\n", (char*)val);
    printf("hm length: %d\n", hm->length);

    freeHashMap(hm, NULL);

    return 0;
}
