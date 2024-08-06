/* 
 * crawler.c -- implementation of crawler in 4 functions
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
#include "../libcs50/mem.h"

/* function prototype *declaration*; the function is *defined* below */
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);


/* ************** main *************** */
int 
main (const int argc, char* argv[]) {

    char* seedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth = 0;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s [seedURL] [pageDirectory] [maxDepth]\n", argv[0]);
        return 1;
    }

    // checks for any argument errors
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);

    // arguments are checked before passed into crawl
    crawl(seedURL, pageDirectory, maxDepth);

    return 0;
}

/* ********* parseArgs ********* */
/* see crawler.h for description */
static void 
parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth) {

    // check if there are the right numbers of input
    if (argc != 4) {
        fprintf(stderr, "Usage: %s [seedURL] [pageDirectory] [maxDepth]\n", argv[0]);
        exit(1);
    }

    *seedURL = argv[1];
    *pageDirectory = argv[2];
    *maxDepth = atoi(argv[3]); // convert the string to an integer
    char* internalURL = normalizeURL("http://cs50tse.cs.dartmouth.edu/tse/");

    // checking if seedURL is valid and internal 
    char* normURL = normalizeURL(*seedURL); // returns string if succesfull, NULL if not
    if (normURL != NULL) { // check if valid URL

        if (strncmp(normURL, internalURL, strlen(internalURL)) == 0) { // check if URL starts correctly
            *seedURL = normURL; // Set the pointer to the normalized URL
        } else {
            fprintf(stderr, "SeedURL     Not Internal: %s\n", normURL); // URL is not internal
            free(normURL);
            free(internalURL);
            exit(1); 
        }

    } else {
        fprintf(stderr, "SeedURL     InvalidURL: %s\n", *seedURL); // URL is invalid
        free(normURL);
        free(internalURL);
        exit(1);
    }

    // check if can create a file .crawler in pageDirectory
    if (pagedir_init(*pageDirectory)) { // pagedir_init() is responsible for returning true or false
    } else {
        fprintf(stderr, "PageDirectory     Unable to create a file of form %s/.crawler\n", *pageDirectory);
        exit(1);
    }

    // checking if maxDepth is valid and in range
    if (*maxDepth >= 0 && *maxDepth <= 10) {
    } else {
        fprintf(stderr, "maxDepth %d out of range [0, 10]\n", *maxDepth);
        exit(1);
    }

    free(internalURL);

}

/* ************ crawl ************/
/* see crawler.h for description */
static void 
crawl(char* seedURL, char* pageDirectory, const int maxDepth) {

    // check if all arguments are valid
    // would most likely not happen since already checked in parseArgs
    if (seedURL == NULL || pageDirectory == NULL || maxDepth < 0 || maxDepth > 10) {
        return;
    }

    // create webpage with seedURL
    webpage_t* web = webpage_new(seedURL, 0, NULL);

    // docID for each file in filePath
    int docID = 1;

    // create a bag and insert into bag if bag is valid
    bag_t* pagesToCrawl = bag_new();
    if (pagesToCrawl != NULL) {
        bag_insert(pagesToCrawl, web);
    } else {
        fprintf(stderr, "Memory error: bag_new() failed\n");
        exit(1);
    }
    
    // create a hashtable and insert into hashtable if it's valid
    hashtable_t* pagesSeen = hashtable_new(200); // 200 determined by IMPLEMENTATION.md
    if (pagesSeen != NULL) {
        hashtable_insert(pagesSeen, seedURL, "");
    } else {
        fprintf(stderr, "Memory error: hashtable_new() failed\n");
        exit(1);
    }

    // loop through the bag while it isn't empty
    webpage_t* current;
    while ((current = bag_extract(pagesToCrawl)) != NULL) {

        if (webpage_fetch(current)) { // success
            fprintf(stdout, "%d     Fetched: %s\n", webpage_getDepth(current), webpage_getURL(current));
            pagedir_save(current, pageDirectory, docID); // will exit with 1 if cannot save page
            docID++; // increment file name
            if (webpage_getDepth(current) < maxDepth) { // only scans for the next URLs if not maxDepth yet
                fprintf(stdout, "%d     Scanning: %s\n", webpage_getDepth(current), webpage_getURL(current));
                pageScan(current, pagesToCrawl, pagesSeen);
            } else {
            }

        } else { // unsuccessful fetch
            return;
        }
        webpage_delete(current);
    }
    bag_delete(pagesToCrawl, webpage_delete);
    hashtable_delete(pagesSeen, NULL);
}


/* ************ pageScan ************/
/* see crawler.h for description */
static void 
pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {

    int pos = 0;
    char* result;
    char* internalURL = normalizeURL("http://cs50tse.cs.dartmouth.edu/tse/");

    while ((result = webpage_getNextURL(page, &pos)) != NULL) { // not sure if 0
        char* normURL = normalizeURL(result); // URL for next page; returns string if succesfull, NULL if not
        if (normURL != NULL) {
            fprintf(stdout, "%d     Found: %s\n", webpage_getDepth(page), normURL);

            if (strncmp(normURL, internalURL, strlen(internalURL)) == 0) { // internal URL
                if (hashtable_find(pagesSeen, normURL) == NULL) { // is not already seen
                    hashtable_insert(pagesSeen, normURL, "");

                    if (hashtable_find(pagesSeen, normURL) != NULL) { // can find it in pagesSeen
                                fprintf(stdout, "%d     Added: %s\n", webpage_getDepth(page), normURL);
                                webpage_t* newPage = webpage_new(normURL, webpage_getDepth(page) + 1, NULL);
                                bag_insert(pagesToCrawl, newPage);

                    }
                } else {
                    fprintf(stdout, "%d     Ignore duplicate: %s\n", webpage_getDepth(page), normURL);
                    mem_free(normURL);
                }
            } else {
                fprintf(stdout, "%d     Ignore external: %s\n", webpage_getDepth(page), normURL);
                mem_free(normURL);
            }
            mem_free(result); 
        } else {
            fprintf(stdout, "%d     NormURL     InvalidURL: %s\n", webpage_getDepth(page), normURL); // URL is invalid
            mem_free(normURL);
        }
    }
    mem_free(internalURL);
}
