/* 
 * counters.c - CS50 'counters' module
 *
 * see counters.h for more information.
 *
 * Memo Tangtipongkul, Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "counters.h"
#include "mem.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct cnode {
    int key; // key
    int count; // count that can be incremented
    struct cnode *next; // points to next node
} cnode_t;

/**************** global types ****************/
typedef struct counters {
    struct cnode *head;       // head of the list in counters
} counters_t;


/**************** local functions ****************/
/* not visible outside this file */
static cnode_t* cnode_new(const int key, int count); // pointer to new node in set

/**************** setnode_new ****************/
/* Allocate and initialize a bagnode */
static cnode_t*  // not visible outside this file
cnode_new(const int key, int count) {
    if (key >= 0) { // check if valid -- has to be non-negative

        cnode_t* new = mem_malloc(sizeof(cnode_t)); // new node
#ifdef MEMTEST
    mem_report(stdout, "During cnode_new");
#endif
        new->key = key;
        new->count = count; // assuming count is 1
        new->next = NULL;
        return new;

    } else {
        return NULL;
    }
}


/**************** global functions ****************/
/* that is, visible outside this file */
/* see counters.h for comments about exported functions */

/**************** counters_new ****************/
/* see counters.h for description */
counters_t* 
counters_new(void) {

    counters_t* counters = mem_malloc(sizeof(counters_t));

#ifdef MEMTEST
    mem_report(stdout, "During counters_new");
#endif

    if (counters == NULL) {
        return NULL;
    } else {
        counters->head = NULL; // initially points to null
        return counters;
    }
}

/**************** counters_add ****************/
/* see counters.h for description */
int 
counters_add(counters_t* ctrs, const int key) {

    if (ctrs != NULL && key >= 0) { // check if valid
         
        // try to retrieve count at key
        int count = counters_get(ctrs, key);
        if (count != 0) { // can retrieve count
            counters_set(ctrs, key, ++count); // set to appropriate count
            return count; // return updated count
        } else { // key doesn't exist
            cnode_t* new = cnode_new(key, 1); // create new node

            // point head to new node and return count
            if (new != NULL) {
                new->next = ctrs->head;
                ctrs->head = new;
                return new->count; 
            } else {
                return 0;
            }
        }

    } else {
        return 0;
    }
}

/**************** counters_get ****************/
/* see counters.h for description */
int 
counters_get(counters_t* ctrs, const int key) {
    if (ctrs != NULL && key >= 0) { // check if valid

        for (cnode_t* node = ctrs->head; node != NULL; node = node->next) {
            if (key == node->key) {
                return node->count;
            }
        }
        return 0;

    } else {
        return 0;
    }
}

/**************** counters_set ****************/
/* see counters.h for description */
bool 
counters_set(counters_t* ctrs, const int key, const int count) {

    if (ctrs != NULL && key >= 0 && count >= 0) { // check if valid

        // see if key exists -- update count if exsits
        for (cnode_t* node = ctrs->head; node != NULL; node = node->next) {
            if (node->key == key) {
                node->count = count;
                return true;
            }
        }
        // key doesn't exist so create new countersnode
        cnode_t* new = cnode_new(key, count);

        if (new != NULL) {
            new->next = ctrs->head;
            ctrs->head = new;
            return true; 
        } else {
            return false;
        }

    } else {
        return false;
    }
}

/**************** counters_print ****************/
/* see counters.h for description */
void 
counters_print(counters_t* ctrs, FILE* fp) {
    if (fp != NULL) { // prints nothing if fp is null
        
        if (ctrs != NULL) { 

            fputc('{', fp); // surround by bracket
            // loop through and print in format
            for (cnode_t* node = ctrs->head; node != NULL; node = node->next) {
                fprintf(fp, "%d=%d", node->key, node->count);
                fputs(", ", fp);
            }
            fputc('}', fp);

        } else {
            fputs("(null)", fp); // prints no item if ctrs is null
        }
    }
}

/**************** counters_iterate ****************/
/* see counters.h for description */
void 
counters_iterate(counters_t* ctrs, void* arg, void (*itemfunc)(void* arg, const int key, const int count)) {
    if (ctrs != NULL && itemfunc != NULL) { // check if valid

        // loop through and call itemfunc on all the nodes
        for (cnode_t* node = ctrs->head; node != NULL; node = node->next) {
            (*itemfunc)(arg, node->key, node->count);
        }

    }
}

/**************** counters_delete ****************/
/* see counters.h for description */
void 
counters_delete(counters_t* ctrs) {
    if (ctrs != NULL) {
        for (cnode_t* node = ctrs->head; node != NULL; ) {
            cnode_t* next = node->next;
            mem_free(node);
            node = next;
        }
        mem_free(ctrs);
    }
#ifdef MEMTEST
    mem_report(stdout, "End of counters_delete");
#endif
}
