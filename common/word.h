/* 
 * pagedir.h - header file for CS50 pagedir module
 *
 *
 * Memo Tangtipongkul, Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>

/**************** word_normalize ****************/
/* Construct pathname for .crawler file in directory
 * opens file to write and closes file
 * Called in crawler.c parseArgs function.
 *
 * We return:
 *   true if filePath was created succesfully and can write into the file
 *   false otherwise
 */
char* word_normalize(const char* word);