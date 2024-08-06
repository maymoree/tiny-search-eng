/* 
 * index.c - CS50 'index' module
 *
 * see index.h for more information.
 * 
 * Memo Tangtipongkul, Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"

/**************** file-local global variables ****************/
/* none */


/**************** local types ****************/
/* none */

/**************** global types ****************/
typedef hashtable_t index_t;


/**************** local functions ****************/
static void item_delete(void* item);
void pair_print(void* arg, const int key, const int count);
void line_print(void* arg, const char* key, void* item);

/**************** global functions ****************/
/* that is, visible outside this file */
/* see index.h for comments about exported functions */

/**************** index_new ****************/
/* see index.h for description */
index_t*
index_new(const int num_slots) {

    // hashtable_new() checks for argument validity already
    return (index_t*)hashtable_new(num_slots); // returns null if invalid
}

/**************** index_insert ****************/
/* see index.h for description */
bool
index_insert(index_t* index, const char* key, counters_t* counters) {

    // check if valid (double check)
    if (index != NULL && key != NULL && counters != NULL) {
        // false means key is found in set, or setnode is NULL
        return hashtable_insert((hashtable_t*)index, key, counters);
    }

    return false;
}

/**************** index_find ****************/
/* see index.h for description */
counters_t*
index_find(index_t* index, const char* key) {

    // check if valid
    if (index != NULL && key != NULL) {
        // NULL means didn't find key in set_find(); stored in set are counters
        return (counters_t*)hashtable_find((hashtable_t*)index, key); // convert to counters_t
    }

    return NULL;
}

/**************** index_delete ****************/
/* see index.h for description */
void index_delete(index_t *index) {

    hashtable_delete((hashtable_t*)index, item_delete);

}

/**************** index_save ****************/
/* see index.h for description */
bool
index_save(index_t* index, char* index_file) {

    if(index != NULL && index_file != NULL) {

        FILE* fp = fopen(index_file, "w");
        hashtable_iterate((hashtable_t*)index, fp, line_print);
        fclose(fp);
        return true;
    }

    return false;
}

/**************** index_load ****************/
/* see index.h for description */
index_t*
index_load(char* index_file) {

    if (index_file == NULL) {
        return NULL;
    }

    FILE* fp = fopen(index_file, "r");
    if (fp == NULL) {
        fclose(fp);
        return NULL;
    }

    index_t* index = index_new(file_numLines(fp)); // creating index by accounting for lines in files
    if (index == NULL) {
        return NULL;
    }

    int key;
    int count;
    char* word;
    // loops through each word
    while ((word = file_readWord(fp)) != NULL) {

        counters_t* counters = counters_new();

        // scans all pairs for each word
        while (fscanf(fp, "%d %d", &key, &count) == 2) {
            counters_set(counters, key, count);
        }

        index_insert(index, word, counters);
        free(word);
    }

    fclose(fp);
    return index;

}


/**************** item_delete ****************/
/* deletes each item in the index */
static void
item_delete(void* item) {

    counters_t* counters = item;

    if (counters != NULL){
        counters_delete(item);
    }

}

/**************** pair_print ****************/
/* prints format for each counter pair */
void
pair_print(void* arg, const int key, const int count) {

    FILE* fp = arg;
    fprintf(fp, "%d %d ", key, count);
    
}

/**************** line_print ****************/
/* prints format for each line */
void
line_print(void* arg, const char* key, void* item) {

    FILE* fp = arg;
    counters_t* counters = item;

    fprintf(fp, "%s ", key);
    counters_iterate(counters, fp, pair_print);
    fprintf(fp, "\n");

}