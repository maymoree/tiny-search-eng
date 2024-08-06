/* 
 * settest.c - test program for CS50 set module
 *
 * CS50, Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "hashtable.h"
#include "file.h"
#include "mem.h"

static void htPrint(FILE* fp, const char* key, void* item);
static void htCount(void* arg, const char* key, void* item);
static void htDelete(void* item);

/**************** main ****************/
int
main () {

    hashtable_t* ht1 = NULL;
    int size = 8;
    int paircount;

    printf("Create new hashtable...\n");
    ht1 = hashtable_new(size);
    if (ht1 == NULL) {
        fprintf(stderr, "hashtable_new failed\n");
        return 1;
    }

    // not freed properly here
    printf("\nTESTING HASHTABLE_INSERT & HASHTABLE_FIND\n");
    printf("Test with null ht, good key and item...\n");
    hashtable_insert(NULL, "A", "firstitem");
    printf("Test with null key only...\n");
    hashtable_insert(ht1, NULL, "seconditem");
    printf("Test with null item only...\n");
    hashtable_insert(ht1, "B", NULL); 
    printf("Test with all good...\n");
    hashtable_insert(ht1, "g", "-2");
    printf("Test with duplicated key...\n");
    hashtable_insert(ht1, "g", "dupe");

    printf("\nTESTING HASHTABLE_PRINT\n");
    printf("Test with null ht, good file and func...\n");
    hashtable_print(NULL, stdout, htPrint);
    printf("\nTest with good ht, null file and good func...\n");
    hashtable_print(ht1, NULL, htPrint);
    printf("Test with good ht and file and null func...\n");
    hashtable_print(ht1, stdout, NULL);
    printf("\nTest with all good...\n");
    printf("The hashtable:\n");
    hashtable_print(ht1, stdout, htPrint);
    printf("\n");

    printf("Insert more pairs");
    hashtable_insert(ht1, "A", "Apple");
    hashtable_insert(ht1, "b", "B0Y");
    hashtable_insert(ht1, "C", "C@T");
    hashtable_insert(ht1, "D", "5");
    hashtable_insert(ht1, "F", "13");
    hashtable_insert(ht1, "H", "haha");
    hashtable_insert(ht1, "i", "__?");
    hashtable_insert(ht1, "z", "zebra?");
    hashtable_insert(ht1, "x", "cs50!");
    hashtable_insert(ht1, "y", "cool");


    printf("\nThe hashtable:\n");
    hashtable_print(ht1, stdout, htPrint);
    printf("\n");

    printf("\nTESTING HASHTABLE_ITERATE\n");
    printf("Test with null ht, good arg and func...\n");
    paircount = 0;
    hashtable_iterate(NULL, &paircount, htCount);
    printf("%d\n", paircount);
    printf("Test with good set, null arg and good func...\n");
    paircount = 0;
    hashtable_iterate(ht1, NULL, htCount);
    printf("%d\n", paircount);
    printf("Test with good set and arg and bad func...\n");
    paircount = 0;
    hashtable_iterate(ht1, &paircount, NULL);
    printf("%d\n", paircount);
    printf("Test with all good...\n");
    printf("Count (should be 11): ");
    paircount = 0;
    hashtable_iterate(ht1, &paircount, htCount);
    printf("%d\n", paircount);

    printf("\nTESTING HASHTABLE_DELETE\n");
    printf("Test with bad set and good func...\n");
    hashtable_delete(NULL, htDelete);
    printf("Test with good set and bad func...\n");
    hashtable_delete(ht1, NULL);
    printf("Test with all good...\n");
    printf("delete the set...\n");
    hashtable_delete(ht1, htDelete);
    printf("DONE!\n");

    return 0;
}

/*
* print (key, item) of pairs in sets -- like in setPrint()
*/ 
static void 
htPrint(FILE* fp, const char* key, void* item) {

    // print (key, item)
    // print null instead if NULL
    fprintf(fp, "(");
    if (key != NULL) { // check if valid
        fprintf(fp, "%c", *key);
    } else {
        fprintf(fp, "NULL");
    }
    fprintf(fp, ",");
    if (item != NULL) { // check if valid
        fprintf(fp, "%s",(char*) item);
    } else {
        fprintf(fp, "NULL");
    }
    fprintf(fp, ")");

}

/*
* count non-null pairs in set
* doesn't matter what is stored in the pair -- like in setCount()
*/
static void 
htCount(void* arg, const char* key, void* item) {

    int* npairs = arg;

    if (npairs != NULL && key != NULL && item != NULL) {
        (*npairs)++;
    }

}

/* 
* delete the item -- like in setDelete()
 */
static void 
htDelete(void* item) {
    return;
}

