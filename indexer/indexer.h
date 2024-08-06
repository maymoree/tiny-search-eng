/* 
 * indexer.h - header file for CS50 indexer module
 *
 *
 * Memo Tangtipongkul, Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../common/pagedir.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../common/index.h"
#include "../common/word.h"

/**************** main ****************/
/* We return:
 * 0 if indexer.c succesfully runs
 * 1 if there are any errors
 */
int main(const int argc, char* argv[]);

/**************** indexBuild ****************/
/* Builds the index as it goes through all the files in pageDirectory
 * Called in main.
 *
 * We return:
 *   an index
 * We exit with error 1 if:
 * - there is an invalid filePath in the pageDirectory
 * - cannot read pageDirectory/1
 */
index_t* indexBuild(char* pageDirectory);

/**************** indexPage ****************/
/* Adds the words and counters into the index accordingly
 *
 * We return:
 *   nothing
 * We exit with error 1 if:
 * - webpage has no html (null)
 * - word cannot be normalized
 * - cannot create new counterset
 * - cannot add new counterset
 */
void indexPage(index_t* index, webpage_t* page, int docID);