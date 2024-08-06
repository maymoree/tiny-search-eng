/* 
 * word.c - CS50 'word' module
 *
 * see word.h for more information.
 * 
 * Memo Tangtipongkul, Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**************** file-local global variables ****************/
/* none */


/* ************ word_normalize ************/
/* see word.h for description */
char*
word_normalize(char* word) {

    if (word == NULL) {
        return NULL;
    }

    // loop through each character and change it to lower
    for (int i = 0; i < strlen(word); i++) {
        char c = word[i];
        word[i] = tolower(c);
    }

    return word;

}