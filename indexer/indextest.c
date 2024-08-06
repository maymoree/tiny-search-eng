/* 
 * indexer.c -- implementation of indexer
 * 
 * Memo Tangtipongkul, Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/index.h"

int
main (const int argc, const char* argv[]) {

    // check if correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s [oldIndexFilename] [newIndexFilename]\n", argv[0]);
        return 1;
    }

    char* oldIndex = calloc(strlen(argv[1]) + 1, sizeof(char));
    char* newIndex = calloc(strlen(argv[2]) + 1, sizeof(char));
    strcpy(oldIndex, argv[1]);
    strcpy(newIndex, argv[2]);

    // check conditions for both filePaths
    if (oldIndex == NULL) {
        fprintf(stderr, "Error in main: invalid oldIndexFilename");
        exit(1);
    }
    if (newIndex == NULL) {
        fprintf(stderr, "Error in main: invalid newIndexFilename");
        exit(1);
    }

    // create new index from oldIndexFilePath
    index_t* index = index_load(oldIndex);
    // save it to the new file path
    if (index_save(index, newIndex) == false) {
        fprintf(stderr, "Error in main: index_save() failed\n");
        exit(1);
    }

    index_delete(index);
    free(oldIndex);
    free(newIndex);

    return 0;

}