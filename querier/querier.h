/* 
 * querier.h - header file for CS50 indexer module
 *
 *
 * Memo Tangtipongkul, Fall 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> 
#include <ctype.h>
#include "../common/pagedir.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../common/index.h"
#include "../common/word.h"

/* local structures */
typedef struct twoctrs {
    counters_t* c_out;
    counters_t* c_in;
} twoctrs_t;

typedef struct maxpair {
    int max_key;
    int max_count;
} maxpair_t;

/**************** main ****************/
/* We return:
 * 0 if querier.c succesfully runs
 * 1 if invalid command-line arguments
 * 1 if cannot parse arguments
 */
int main (int argc, const char* argv[]);

/**************** parseArg ****************/
/* Valids and parses the arguments
 * Called in main.
 *
 * We return:
 *   true if there are no errors in parsing
 *   false if:
 *      invalid command-line arguments,
 *      unable to read a file named pageDirectory/.crawler
 *      unable to read a file named pageDirectory/1
 *      unable to read a file named indexFilename
 */
bool parseArg(const int argc, const char* argv[], char* pageDirectory, char* indexFilename);

/**************** takeQuery ****************/
/* Mainly runs the whole program: validates, unionizes/intersect finds, and sorts matches
 * Called in main. 
 * 
 * We return:
 *   nothing
 */
void takeQuery(char* pageDirectory, index_t* index);



/**************** parseQuery ****************/
/* Normalizes and parses the words in query (gets rid of white space).
 * Adds all the cleaned words into an array.
 * Called in takeQuery.
 *
 * We return:
 *   wordCounter -- the number of words in the cleaned query
 */
int parseQuery(char* query, char* query_arr[]);

/**************** checkQuery ****************/
/* Checks the syntax of the cleaned query in the array.
 * Called in takeQuery.
 *
 * We return:
 *   1 if the syntax is invalid
 *   0 if the syntax is valid
 */
int checkQuery(char* query_arr[]);



/**************** c_union ****************/
/* Iterates through counter c_in; calls c_union_helper (for <orsequence>)
 * Called in takeQuery.
 *
 * We return:
 *   nothing
 */
void c_union(counters_t* c_out, counters_t* c_in);

/**************** c_union_helper ****************/
/* Helps add all the keys from c_in into c_out
 * Called in c_union.
 *
 * We return:
 *   nothing
 */
void c_union_helper(void* arg, const int key, const int count);

/**************** c_intersect ****************/
/* Iterates through counter c_out; calls c_intersect_helper (for <andsequence>)
 * Called in takeQuery.
 *
 * We return:
 *   nothing
 */
void c_intersect(counters_t* c_out, counters_t* c_in);

/**************** c_intersect_helper ****************/
/* If word (key) that exists in c_out exists in c_in, set the count of c_out to the minimum between the two
 * Otherwise set the count to 0 
 * Called in c_intersect.
 *
 * We return:
 *   nothing
 */
void c_intersect_helper(void* arg, const int key, const int count);



/**************** count_counters ****************/
/* Counts how many pairs (key, count) are in the counters
 * Called in takeQuery.
 *
 * We return:
 *   nothing
 */
void count_counters(void* arg, const int key, const int count);

/**************** count_matches ****************/
/* Counts how many pairs (key, count) of count value more than 0 are in the counters
 * Called in takeQuery.
 *
 * We return:
 *   nothing
 */
void count_matches(void* arg, const int key, const int count);

/**************** sort_max ****************/
/* Iterates through the counter to find the max, print it out, and set its count to 0
 * Called in takeQuery.
 *
 * We return:
 *   0 if successful
 */
int sort_max(counters_t* c_output, int num_pairs, char* pageDirectory);

/**************** sort_max_helper ****************/
/* Helps find the max score (count)
 * Called in sort_max.
 *
 * We return:
 *   nothing
 */
void sort_max_helper(void* arg, const int key, const int count);

/**************** print_score ****************/
/* Prints out the score, document and URL in a formatted way
 * Called in sort_max.
 *
 * We return:
 *   0 if successful
 */
int print_score(char* pageDirectory, maxpair_t* maxpair);