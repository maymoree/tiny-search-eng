/* 
 * set.c - CS50 'set' module
 *
 * see set.h for more information.
 *
 * Memo Tangtipongkul, Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"
#include "mem.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct setnode {
    char* key; // key
    void* item; // value that is linked to key
    struct setnode *next; // points to next node
} setnode_t;

/**************** global types ****************/
typedef struct set {
    struct setnode *head;       // head of the list of items in set
} set_t;


/**************** local functions ****************/
/* not visible outside this file */
static setnode_t* setnode_new(const char* key, void* item); // pointer to new node in set

/**************** setnode_new ****************/
/* Allocate and initialize a bagnode */
static setnode_t*  // not visible outside this file
setnode_new(const char* key, void* item)
{
    if (key != NULL && item != NULL) { // check if valid
        setnode_t* new = mem_malloc(sizeof(setnode_t)); // new node

        // add key and item
        new->key = mem_malloc(strlen(key)+1); // tree.c - allocate memory first since const 
        new->item = item; 
        
        // copy key into new key
        strcpy(new->key,key); // copy into allocated memory

        new->next = NULL;
        return new;
    } else {
        return NULL;
    }
}


/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */
// set_t* set_new(void);
// bool set_insert(set_t* set, const char* key, void* item);
// void* set_find(set_t* set, const char* key);
// void set_print(set_t* set, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item));
// void set_iterate(set_t* set, void* arg, void (*itemfunc)(void* arg, const char* key, void* item));
// void set_delete(set_t* set, void (*itemdelete)(void* item));

/**************** set_new ****************/
/* see set.h for description */
set_t*
set_new(void) {
    set_t* set = mem_malloc(sizeof(set_t));

    if (set == NULL) { // check if valid
        return NULL;
    } else {
        set->head = NULL; // head pointer points to null
        return set;
    }
}

/**************** set_insert ****************/
/* see set.h for description */
bool
set_insert(set_t* set, const char* key, void* item) {

    if (set != NULL && key != NULL && item != NULL) { // check if all are valid

        if (set_find(set, key) == NULL) { // check key doesn't already exist/not found

            setnode_t* new = setnode_new(key, item); // new node
            if (new != NULL) { 
                new->next = set->head; // points to null
                set->head = new; // points to new
                #ifdef MEMTEST
                    mem_report(stdout, "Running set_insert");
                #endif
                return true;
            } else {
                return false;

            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

/**************** set_find ****************/
/* see set.h for description */
void*
set_find(set_t* set, const char* key){
    if (set != NULL && key != NULL) { // check if all are valid

        if (set->head != NULL) { // head is not pointing to null

            for (setnode_t* node = set->head; node != NULL; node = node->next) {
                if (strcmp((char*)node->key, (char*)key) == 0) { // key is found
                    return node->item; // returns item of key
                }
            }
            return NULL; // loops through and didn't find key
            
        } else {
            return NULL;
        }

    }
    return NULL;
}

/**************** set_print ****************/
/* see set.h for description */
void
set_print(set_t* set, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item)){
    if (fp != NULL) { // if null then prints nothing

        if (set != NULL) { 

            fputc('{', fp); // surround by bracket
            for (setnode_t* node = set->head; node != NULL; node = node->next) {

                // print node
                if (itemprint != NULL) { // check if function is valid
                    (*itemprint)(fp, node->key, node->item); // print pair
                    fputc(',', fp); // comma separated
                }
            }
            fputc('}', fp);

        } else {
            fputs("(null)", fp); // prints no item if set is null
        }
    }
}

/**************** set_iterate ****************/
/* see set.h for description */
void 
set_iterate(set_t* set, void* arg, void (*itemfunc)(void* arg, const char* key, void* item)){
    if (set != NULL && itemfunc != NULL){
        fprintf(stdout, "iterating\n");

        for (setnode_t* node = set->head; node != NULL; node = node->next) { // loop trough
            (*itemfunc)(arg, node->key, node->item); // call function on node
        }

    } else {
        fprintf(stdout, "can't interate\n");
    }
}

/**************** set_delete ****************/
/* see set.h for description */
void 
set_delete(set_t* set, void (*itemdelete)(void* item)) {
    if (set != NULL && itemdelete != NULL) { // check if set is valid 
        
        // delete each node if function is valid
        for (setnode_t* node = set->head; node != NULL; ){

            if (node->item != NULL) {
                (*itemdelete)(node->item); // call function on each node
            }
            setnode_t* next = node->next; // store the next node of current node
            mem_free(node->key);
            mem_free(node); // free node
            node = next; // move to next node
        }

        mem_free(set); // free set
    }

#ifdef MEMTEST
    mem_report(stdout, "End of set_delete");
#endif
}
