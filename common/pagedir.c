/* 
 * pagedir.c - CS50 'pagedir' module
 *
 * see pagedir.h for more information.
 * 
 * Memo Tangtipongkul, Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/**************** file-local global variables ****************/
/* none */


/* ************ pagedir_init ************/
/* see pagedir.h for description */
bool 
pagedir_init(const char* pageDirectory) {

    char filePath [1000];
    sprintf(filePath, "%s/.crawler", pageDirectory); // create path for .crawler in directory

    FILE* fp = fopen(filePath, "w");

    if (fp != NULL) { // can write in the file
        fclose(fp);
        return true;
    } 

    // fprintf(stderr, "Cannot open file: %s\n", filePath);
    return false;

}


/* ************ pagedir_save ************/
/* see pagedir.h for description */
void 
pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {

    char filePath [1000];
    sprintf(filePath, "%s/%d", pageDirectory, docID); // create filePath

    FILE* fp = fopen(filePath, "w"); // write into file

    if (fp != NULL) { // can write into file
        char* URL = webpage_getURL(page);
        char* HTML = webpage_getHTML(page);
        int depth = webpage_getDepth(page);

        fprintf(fp, "%s\n", URL);
        fprintf(fp, "%d\n", depth);
        fprintf(fp, "%s\n", HTML);
        
        fclose(fp); // close file
    // error otherwise
    } else {
        fprintf(stderr, "Cannot open file: %s\n", filePath);
        exit(1);
    }
    
}

/* ************ pagedir_validate ************/
/* see pagedir.h for description */
bool 
pagedir_validate(const char* pageDirectory) {

    char filePath [1000];
    sprintf(filePath, "%s/.crawler", pageDirectory); // create path for .crawler in directory

    FILE* fp = fopen(filePath, "r");

    if (fp != NULL) { // can open the file, therefore it is a crawler directory
        fclose(fp);
        return true;
    } 

    // cannot open file, not a crawler directory
    return false;

}

/* ************ pagedir_load ************/
/* see pagedir.h for description */
webpage_t*
pagedir_load(const char* pageDirectory, const int docID) {

    char filePath [1000];
    sprintf(filePath, "%s/%d", pageDirectory, docID); // create filePath

    FILE* fp = fopen(filePath, "r");

    if (fp == NULL) { // unable to open file
        return NULL;
    }

    // read lines 
    char* url = file_readLine(fp);
    char* raw_depth = file_readLine(fp);
    char* html = file_readFile(fp);

    int depth = atoi(raw_depth);
    
    webpage_t* page = webpage_new(url, depth, html);
    fclose(fp);
    
    free(raw_depth);
    return page;
}