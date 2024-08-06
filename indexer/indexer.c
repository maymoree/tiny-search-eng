/* 
 * indexer.c -- implementation of indexer
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

/* function prototype *declaration*; the function is *defined* below */
int main(const int argc, char* argv[]);
index_t* indexBuild(char* pageDirectory);
void indexPage(index_t* index, webpage_t* page, int docID);


/* ************** main *************** */
int 
main (const int argc, char* argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Usage: %s [pageDirectory] [indexFilename]\n", argv[0]);
        return 1;
    }

    // check pageDirectory
    char* pageDirectory = mem_calloc(strlen(argv[1]) + 1, sizeof(char));
    strcpy(pageDirectory, argv[1]);

    if (pageDirectory == NULL) {
        fprintf(stderr, "Invalid in main: pageDirectory\n");
        mem_free(pageDirectory);
        return 1;
    }

    if (pagedir_validate(pageDirectory) == false) {
        // returns unable to read a pageDirectory/.crawler file
        fprintf(stderr, "Invalid in main: cannot read pageDirectory/.crawler\n");
        mem_free(pageDirectory);
        return 1;
    }

    // check indexFilename
    char* indexFilename = mem_calloc(strlen(argv[2]) + 1, sizeof(char));
    strcpy(indexFilename, argv[2]);

    FILE* fp;
    fp = fopen(indexFilename, "w");
    if (fp == NULL) {
        mem_free(indexFilename); 
        mem_free(pageDirectory);
        fprintf(stderr, "Invalid in main: indexFilename cannot write");
        return 1;
    }
    fclose(fp);

    if (indexFilename == NULL) {
        mem_free(pageDirectory);
        mem_free(indexFilename); 
        fprintf(stderr, "Invalid in main: indexFilename\n");
        return 1;
    }

    // call indexBuild
    index_t* index = indexBuild(pageDirectory);

    if (!index_save(index, indexFilename)) {
        index_delete(index);
        mem_free(pageDirectory);
        mem_free(indexFilename);
        fprintf(stderr, "Error in main: cannot write indexFilename\n");
        return 1;
    }

    index_delete(index);
    mem_free(pageDirectory);
    mem_free(indexFilename);

    return 0;
}

/* ********* indexBuild ********* */
/* see index.h for description */
index_t*
indexBuild(char* pageDirectory) {

    // create an index (hashtable) with a size of 200
    index_t* index = index_new(200);

    int docID = 1;
    char* filePath = mem_calloc(strlen(pageDirectory) + 5, sizeof(char));    
    sprintf(filePath, "%s/%d", pageDirectory, docID);

    fprintf(stdout, "%s\n", filePath);

    if (filePath == NULL) {
        fprintf(stderr, "Invalid in indexBuild: filePath\n");
        exit(1);
    }

    // unable to read file pageDirectory/1
    FILE* file1;
    file1 = fopen(filePath, "r");
    if (file1 == NULL) {
        fprintf(stderr, "Invalid in indexBuild: cannot read pageDirectory/1");
        exit(1);
    }
    fclose(file1);

    FILE* fp;
    while ((fp = fopen(filePath, "r")) != NULL) { // can read and open filePath

        // loops through all the files in the pageDirectory
        // pageDirectory is already validated in main 
        webpage_t* page = pagedir_load(pageDirectory, docID); // creates a webpage from file in crawler directory

        // passes to indexPage to add to index
        indexPage(index, page, docID);

        // updates to the next file in pageDirectory
        docID++;
        sprintf(filePath, "%s/%d", pageDirectory, docID);

        webpage_delete(page);
        fclose(fp);
    }
    mem_free(filePath);
    return index;

}

/* ********* indexPage ********* */
/* see index.h for description */
void
indexPage(index_t* index, webpage_t* page, int docID) {

    // check that html is not null
    if (webpage_getHTML(page) == NULL) {
        fprintf(stderr, "Invalid in indexPage: HTML in page is null\n");
        exit(1);
    }


    char* word;
    int pos = 0;
    while ((word = webpage_getNextWord(page, &pos)) != NULL) {

        if (strlen(word) < 3) {
            mem_free(word);
            continue;
        }

        char* normWord = word_normalize(word);

        if (normWord == NULL) { // check that normalized word is not null
            fprintf(stderr, "Invalid in indexPage: normWord\n");
            mem_free(normWord);
            mem_free(word);
            exit(1);
        }

        counters_t* currCounter = index_find(index, normWord);

        // need to add new counter into the index
        if (currCounter == NULL) {
            counters_t* newCounter = counters_new();

            if (newCounter == NULL) {
                mem_free(normWord);
                mem_free(word);
                fprintf(stderr, "Error in indexPage: counters_new() failed\n");
                exit(1);
            }
            counters_add(newCounter, docID); // add a counter for this word in the current document

            index_insert(index, normWord, newCounter);

        } else { // key (normWord) already exists

            // increment counter and check if it was successful
            if (counters_add(currCounter, docID) == 0) {
                mem_free(normWord);
                mem_free(word);
                fprintf(stderr, "Error in indexPage: counters_add() failed");
                exit(1);
            }
        }
        mem_free(normWord);

    }
    mem_free(word);

}