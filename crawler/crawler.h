/* 
 * crawler.h - header file for CS50 crawler module
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
#include "../libcs50/bag.h"
#include "../libcs50/webpage.h"

/**************** main ****************/
/* We return:
 * 0 if crawler.c succesfully runs
 * 1 if any arguments are invalid
 */
int main(const int argc, char* argv[]);

/**************** parseArgs ****************/
/* Parse the command-line arguments
 * Called in main.
 *
 * We return:
 *   nothing if the command-line arguments are all valid
 * We exit with error 1 if:
 * - invalid number of arguments from command-line
 * - seedURL is invalid or not internal
 * - maxDepth is invalid or out of range
 * - unable to create a file of form pageDirectory/.crawler
 */
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);

/**************** crawl ****************/
/* Crawls through the URLs and saves page information in files under directory
 * Called in main.
 *
 * Arguments that are passed in are checked by parseArgs
 * 
 * We return:
 *   nothing if the arguments are all valid
 * We exit with error 1 if:
 * - out of memory; this is when bag_new or hashtable_new fails to create
 * - unable to create or write to a file of form pageDirectory/id; this is called by pageSave()
 */
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);

/**************** pageScan ****************/
/* Scans the next URLs that are linked in the current page
 * Adds the URLs to the bag so that crawl continues checking URLs
 * Called in crawl.
 *
 * Arguments that are passed in are checked by crawl
 * 
 * We return:
 *   nothing if the arguments are all valid
 * We announce:
 *   when URLs are duplicated or external
 */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);