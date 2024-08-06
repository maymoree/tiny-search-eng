/* 
 * pagedir.h - header file for CS50 pagedir module
 *
 *
 * Memo Tangtipongkul, Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"


/**************** pagedir_init ****************/
/* Construct pathname for .crawler file in directory
 * opens file to write and closes file
 * Called in crawler.c parseArgs function.
 *
 * We return:
 *   true if filePath was created succesfully and can write into the file
 *   false otherwise
 */
bool pagedir_init(const char* pageDirectory);


/**************** pagedir_save ****************/
/* Constructs pathname for page file in pageDirectory
 * opens file to write and closes file
 * Called in crawler.c crawl function.
 *
 * We return:
 *   nothing
 * We exit with 1 if:
 * - unable to create or write to a file of form pageDirectory/id
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_validate ****************/
/* Validates if the directory is Crawler-producted
 * Called in indexer.c crawl function.
 *
 * We return:
 *   true if directory contains .crawler file
 *   false otherwise
 */
bool pagedir_validate(const char* pageDirectory);

/**************** pagedir_load ****************/
/* Loads the page from Crawler directory
 * Called in indexer.c crawl function.
 *
 * We return:
 *   a page in the directory
 *   NULL if filePath doesn't exist
 */
webpage_t* pagedir_load(const char* pageDirectory, const int docID);