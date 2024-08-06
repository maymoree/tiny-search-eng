/* 
 * index.h - header file for CS50 index module
 *
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

/**************** global types ****************/
typedef hashtable_t index_t;

/**************** functions ****************/

/**************** index_new ****************/
/* Creates a new index by calling hashtable_new()
 *
 * We return:
 *   a new index
 */
index_t* index_new(const int num_slots);

/**************** index_insert ****************/
/* Inserts a counter pair at the key (word) into an index
 *
 * We return:
 *   true if sucessfully inserted
 *   false otherwise
 */
bool index_insert(index_t *index, const char* key, counters_t* counters);

/**************** index_find ****************/
/* Finds the item (counterset) at they key (word) in an index
 *
 * We return:
 *   a counterset if found
 *   NULL otherwise
 */
counters_t *index_find(index_t *index, const char *key);

/**************** index_delete ****************/
/* Deletes the index
 *
 * We return:
 *   nothing
 */
void index_delete(index_t *index);

/**************** index_save ****************/
/* Write into a file the correct format of an index
 * uses helper functions to assist with writing the output
 *
 * We return:
 *   true if successfully writen into the file
 *   false otherwise
 */
bool index_save(index_t *index, char* index_file);

/**************** index_load ****************/
/* Reads from an index file and creates an index
 *
 * We return:
 *   an index if successful
 *   NULL otherwise
 */
index_t *index_load(char* index_file);


