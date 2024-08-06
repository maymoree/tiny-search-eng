/* 
 * querier.c -- implementation of querier
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
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../common/index.h"
#include "../common/word.h"
#include "../libcs50/file.h"

/* local structures */
// holds two counters to pass into counter_iterate() (and helper functioins)
typedef struct twoctrs {
    counters_t* c_out;
    counters_t* c_in;
} twoctrs_t;

// holds max key and count to correctly sort the matched documents
typedef struct maxpair {
    int max_key;
    int max_count;
} maxpair_t;

/* function prototype *declaration*; the function is *defined* below */
int main (int argc, const char* argv[]);
bool parseArg(const int argc, const char* argv[], char* pageDirectory, char* indexFilename);
void takeQuery(char* pageDirectory, index_t* index);
int parseQuery(char* query, char* query_arr[]);
int checkQuery(char* query_arr[]);
void c_union(counters_t* c_out, counters_t* c_in);
void c_union_helper(void* arg, const int key, const int count);
void c_intersect(counters_t* c_out, counters_t* c_in);
void c_intersect_helper(void* arg, const int key, const int count);
void count_counters(void* arg, const int key, const int count);
void count_matches(void* arg, const int key, const int count);
int sort_max(counters_t* c_output, int num_pairs, char* pageDirectory);
void sort_max_helper(void* arg, const int key, const int count);
int print_score(char* pageDirectory, maxpair_t* maxpair);

/* ************** main *************** */
int 
main (int argc, const char* argv[]) {

    // check if arguments are valid
    if (argc != 3) {
        fprintf(stderr, "Usage: %s [pageDirectory] [indexFilename]\n", argv[0]);
        return 1;
    }

    // create a pageDirectory pointer
    char* pageDirectory = mem_calloc(strlen(argv[1]) + 1, sizeof(char));
    strcpy(pageDirectory, argv[1]); // example: data/toscrape-2
    // create an indexFilename pointer
    char* indexFilename = mem_calloc(strlen(argv[2]) + 1, sizeof(char)); 
    strcpy(indexFilename, argv[2]); // example: data/toscrape-2.index

    // check that both arguments are valid
    bool parsed = parseArg(argc, argv, pageDirectory, indexFilename);
    if (!parsed) {
        mem_free(pageDirectory);
        mem_free(indexFilename); 
        return 1;
    }

    // proceed with the querier program
    index_t* index = index_load(indexFilename); // loads index from file

    // process of querying and printing out sorted results
    takeQuery(pageDirectory, index);

    // clean
    index_delete(index);
    mem_free(pageDirectory);
    mem_free(indexFilename);
    
    return 0;
}

/* ********* parseArg ********* */
/* see querier.h for description */
bool
parseArg(const int argc, const char* argv[], char* pageDirectory, char* indexFilename) {

    // check amount of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s [pageDirectory] [indexFilename]\n", argv[0]);
        return false;
    }

    // check if pageDirectory is valid
    if (pageDirectory == NULL) { // check if pageDirectory exitsts
        fprintf(stderr, "Invalid in parseArg: pageDirectory\n");
        return false;
    }
    if (pagedir_validate(pageDirectory) == false) { // returns unable to read a pageDirectory/.crawler file
        fprintf(stderr, "Invalid in parseArg: cannot read pageDirectory/.crawler\n");
        return false;
    }

    // check if indexFilename is valid
    if (indexFilename == NULL) { // check if path exists
        fprintf(stderr, "Invalid in parseArg: indexFilename is null\n");
        return false;
    }
    FILE* fp;
    fp = fopen(indexFilename, "r");
    if (fp == NULL) { // check if can read
        fprintf(stderr, "Invalid in parseArg: unable to read indexFilename\n");
        return false;
    }
    fclose(fp);

    return true;

}


///
/// main function for validating, querying, and sorting
///


/* ********* takeQuery ********* */
/* see querier.h for description */
void
takeQuery(char* pageDirectory, index_t* index) {

    char* query;

    fprintf(stdout, "Query? \n");

    while ((query = file_readLine(stdin)) != NULL) {

        //
        // -------------------------------------- VALIDATE --------------------------------------
        //
        // prints out error if query is not valid

        //
        // checks for space and alphabets only
        //
        int is_valid = 1;
        for (int i = 0; i < strlen(query); i++) {
            if (isalpha(query[i]) == 0 && isspace(query[i]) == 0) { // it's not an alphabet AND not a space
                is_valid = 0;
                fprintf(stdout, "Invalid in takeQuery: query cannot be tokenized\n");
                // don't free here just break out and free in the condition
                break;
            }

        }
        // is not valid
        if (is_valid == 0) {
            fprintf(stdout, "Query? \n");
            mem_free(query);
            continue;
        }


        //
        // parses the query and returns number of words in query
        //
        int wordCounter;
        char* query_arr[1000] = {0}; // array to add in the words
        wordCounter = parseQuery(query, query_arr); // returns the number of words in the array
        // parsed array is null or empty
        if (wordCounter == 0 || query_arr[0] == NULL) {
            fprintf(stdout, "Query? \n");
            mem_free(query);
            continue;
        }


        //
        // checks for the syntax of the query (and / or)
        //
        int is_checked;
        is_checked = checkQuery(query_arr);
        // wrong syntax
        if (is_checked == 1) {
            fprintf(stdout, "Query? \n");
            for (int i = 0; i < wordCounter; i++) { // wordCounter is equal to the amount of words in query_arr[]
                if (query_arr[i] != NULL) {
                    mem_free(query_arr[i]);
                }
            }
            mem_free(query);
            continue;
        }

        //
        // prints out the cleaned query
        //
        printf("Query: ");
        int i = 0;
        while (query_arr[i] != NULL) {
            printf("%s ", query_arr[i]);
            i++;
        }
        printf("\n");

        //
        // -------------------------------------- QUERY --------------------------------------
        //

        //
        // check if query has 'or' <orsequence>
        //
        int is_or = false;
        char* or = "or";
        for (int i = 0; i < wordCounter; i++) {
            if (strcmp(query_arr[i], or) == 0) {
                is_or = true;
            }
        }

        //
        // loop through the whole query array to union/intersect counters
        //
        char* and = "and"; // used to compare if word and appears
        bool no_err = true; // remains true if <andsequence> words all find matches
        counters_t* c_output = counters_new(); // ending counter that will be output
        counters_t* c_section = NULL; // sections of the <andsequence>

        for (int i = 0; i < wordCounter;) { // increases i conditionally

            // first case -> word is "and", ignored and moves on to next word in query_arr
            if (strcmp(query_arr[i], and) == 0) {
                i++;
                continue;
            }

            // second case -> word is "or", section is stored in result
            else if (strcmp(query_arr[i], or) == 0) {
                c_union(c_output, c_section);
                counters_delete(c_section);
                c_section = NULL;
            }

            // third case -> any other word, depends on type of sequence and if word is found in index
            else {
                counters_t* c_word = index_find(index, query_arr[i]); // finds the counter for the word itself

                // first case -> word doesn't exist in index and in <andsequence>
                if (c_word == NULL && is_or == false) {

                    no_err = false; // there is an error
                    counters_delete(c_output);
                    counters_delete(c_section);

                    fprintf(stdout, "No documents match.\n");
                    fprintf(stdout, "-----------------------------------------------\n");
                    // don't free here; break out and free in condition
                    break;
                }

                // second case -> word doesn't exist in index and in <orsequence>
                else if (c_word == NULL && is_or == true) {
                    c_word = counters_new(); // create empty counters for the word
                }

                // third case -> word exists
                else {
                    // at the beginning of query OR beginning of <andsequence> (which may be 1 word long)
                    if (c_section == NULL) {
                        c_section = counters_new();
                        c_union(c_section, c_word); 
                    } 
                    // during an <andsequence>
                    else {
                        c_intersect(c_section, c_word);
                    }
                }
            }
            i++;
        }

        //
        // continue to next question if word doesn't have matches in <andsequence>
        //
        if (no_err == false) {

            fprintf(stdout, "Query? \n");
            no_err = true;

            for (int i = 0; i < wordCounter; i++) { // wordCounter is equal to the amount of words in query_arr[]
                if (query_arr[i] != NULL) {
                    mem_free(query_arr[i]);
                }
            }
            mem_free(query);
            continue;
        }

        //
        // finally merge c_section to the c_output
        //
        if (c_section != NULL) {
            c_union(c_output, c_section);
            counters_delete(c_section);
        }


        //
        // -------------------------------------- SORT --------------------------------------
        //

        //
        // counts how many pairs are in the c_output (final) to know how many times to find max
        // prints out the statement for pairs matched
        //
        int num_pairs = 0; // counts every pair
        int matches = 0; // counts only pairs with count more than 0
        counters_iterate(c_output, &num_pairs, count_counters);
        counters_iterate(c_output, &matches, count_matches);

        // need this for <orsequences> that have no matches
        if (matches == 0) {
            fprintf(stdout, "No documents match.\n");
        } else {
            fprintf(stdout, "Matches %d documents (ranked):\n", matches);
        }

        //
        // find max, print it out, and set its count to zero
        //
        sort_max(c_output, num_pairs, pageDirectory);

        fprintf(stdout, "-----------------------------------------------\n");
        
        //
        // cleans the array 
        //
        for (int i = 0; query_arr[i] != NULL; i++) {
            mem_free(query_arr[i]); 
        }

        // clean
        counters_delete(c_output);
        mem_free(query);

        fprintf(stdout, "Query? \n");

    }

}


///
/// helper functions to parse and check query syntax --> used in VALIDATE
///


/* ********* parseQuery ********* */
/* see querier.h for description */
int parseQuery(char* query, char* query_arr[]) {

    // returned to check how many words are in query
    int wordCounter = 0;

    if (query_arr == NULL || query == NULL) {
        return wordCounter;
    }

    // normalize query
    char* query_normal = word_normalize(query);
    if (query_normal == NULL) {
        return wordCounter; // asks for next query in takeQuery
    }

    for (int i = 0; query_normal[i] != '\0';) {
        
        // finds the first non-space character
        while (isspace(query_normal[i]) && query_normal[i] != '\0') {
            i++;
        }

        // ends at end of query
        if (query_normal[i] == '\0') {
            break;
        }

        // set word_start to the first index of non-space character
        int word_start = i;

        // continues finding non-space characters
        while (!isspace(query_normal[i]) && query_normal[i] != '\0') {
            i++;
        }

        // calculates length of the word
        int word_length = i - word_start;

        // copies the word into query_array at index i
        query_arr[wordCounter] = (char*)mem_malloc(word_length + 1);
        strncpy(query_arr[wordCounter], query_normal + word_start, word_length);
        query_arr[wordCounter][word_length] = '\0';

        // increase counter
        wordCounter++;
    }

    return wordCounter;
}

/* ********* checkQuery ********* */
/* see querier.h for description */
int
checkQuery(char* query_arr[]) {
    char* and = "and";
    char* or = "or";
    int i = 0;
    bool and_or = false; // check if previous was and / or

    // start with first word
    if (strcmp(query_arr[i], and) == 0 || strcmp(query_arr[i], or) == 0) {
        fprintf(stderr, "Invalid in checkQuery: cannot begin with and / or\n");
        return 1;
    }

    // loop through the rest of the array
    while (query_arr[i] != NULL) {

        if (and_or) { // if the previous word was and / or
            if (strcmp(query_arr[i], and) == 0 || strcmp(query_arr[i], or) == 0) { // if word is and / or
                fprintf(stderr, "Invalid in checkQuery: cannot have two and / or in a row\n");
                return 1;
            }
            and_or = false; // change and / or to false
        }

        else {
            if (strcmp(query_arr[i], and) == 0 || strcmp(query_arr[i], or) == 0) { // check if word is and / or
                and_or = true;
            }
        }
        i++;
    }

    // end with last word (i-1)
    if (strcmp(query_arr[i-1], and) == 0 || strcmp(query_arr[i-1], or) == 0) {
        fprintf(stderr, "Invalid in checkQuery: cannot end with and / or\n");
        return 1;
    }

    return 0;
}


///
/// helper functions used to union/intersect counters --> used in QUERYING
///


/* ********* c_union ********* */
/* see querier.h for description */
void
c_union(counters_t* c_out, counters_t* c_in) {

    // create a twoctrs_t that holds both counters
    twoctrs_t arg = {c_out, c_in};

    // iterate through c_in to add to c_out
    counters_iterate(c_in, &arg, c_union_helper);

}

/* ********* c_union_helper ********* */
/* see querier.h for description */
void
c_union_helper(void* arg, const int key, const int count) {

    // turn arg into pointer to pass into counters methods
    twoctrs_t* args = arg;

    // add the count from c_in into the count from c_out
    // counters_get returns 0 if the key doesn't exist in c_out
    // does not matter if key doesn't already exist in c_out
    counters_set(args->c_out, key, counters_get(args->c_out, key) + count);

}

/* ********* c_intersect ********* */
/* see querier.h for description */
void
c_intersect(counters_t* c_out, counters_t* c_in) {

    // create a twoctrs_t that holds both counters
    twoctrs_t arg = {c_out, c_in};

    // iterate through c_out to make sure only adds keys that already exist in c_out
    counters_iterate(c_out, &arg, c_intersect_helper);

}

/* ********* c_intersect_helper ********* */
/* see querier.h for description */
void
c_intersect_helper(void* arg, const int key, const int count) {

    // turn arg into pointer to pass into counters methods
    twoctrs_t* args = arg;

    int c_in_count = counters_get(args->c_in, key);
    int c_out_count = counters_get(args->c_out, key);

    // only checks that word in c_out exists in c_in
    if (c_in_count != 0) {
        // finds the min between the current count, and c_in's count
        // sets the current count to the min
        int min;
        min = (c_in_count < c_out_count ? c_in_count : c_out_count);
        counters_set(args->c_out, key, min);
    // if doesn't exist in c_in, set the count of word to 0 in c_out
    } else {
        counters_set(args->c_out, key, 0);
    }
    
}


///
/// helper functions used to sort the output --> used in SORTING
///


/* ********* count_counters ********* */
/* see querier.h for description */
void 
count_counters(void* arg, const int key, const int count) {
    int* num_pairs = arg;
    if (num_pairs != NULL && key >= 0 && count >= 0) { // counts all pairs no matter the count value
        (*num_pairs)++;
    }
}

/* ********* count_matches ********* */
/* see querier.h for description */
void 
count_matches(void* arg, const int key, const int count) {
    int* num_matches = arg;
    if (num_matches != NULL && key >= 0 && count > 0) { // only adds counts that are more than 0
        (*num_matches)++;
    }
}

/* ********* sort_max ********* */
/* see querier.h for description */
int
sort_max(counters_t* c_output, int num_pairs, char* pageDirectory) {

    // will iterate through the c_output according to the amount of pairs in c_output
    for (int i = 0; i < num_pairs; i++) {

        int max_key = 0;
        int max_count = 0;

        maxpair_t* maxpairs = mem_malloc(sizeof(maxpair_t));
        if (maxpairs == NULL) {
            return 1;
        }

        // initially set max_key and max_count to 0
        maxpairs->max_key = max_key;
        maxpairs->max_count = max_count;

        // iterates through the counter and finds the max
        counters_iterate(c_output, maxpairs, sort_max_helper);

        // print out the max
        int print;
        if ((print = print_score(pageDirectory, maxpairs)) == 1) {
            mem_free(maxpairs);
            return 1;
        }

        // set the count of max_key to 0 to prevent it from becoming max again
        if (max_key >= 0) {
            counters_set(c_output, maxpairs->max_key, 0);
        }

        // clean
        mem_free(maxpairs);

    }
    return 0;
    
}

/* ********* sort_max_helper ********* */
/* see querier.h for description */
void
sort_max_helper(void* arg, const int key, const int count) {

    maxpair_t* maxpairs = arg;

    // if the count of current pair is more than max_count, set it to max
    if (count > maxpairs->max_count && count != 0) { // count has to not be zero 
        maxpairs->max_key = key;
        maxpairs->max_count = count;
    }

}

/* ********* print_score ********* */
/* see querier.h for description */
int
print_score(char* pageDirectory, maxpair_t* maxpair) {

    // pageDirectory and document ID (max_key) is already validated
    char* filePath = mem_calloc(strlen(pageDirectory) + 3, sizeof(char));

    // create filePath to get URL of doc
    sprintf(filePath, "%s/%d", pageDirectory, maxpair->max_key);

    FILE* fp = fopen(filePath, "r");

    if (fp == NULL) {
        mem_free(filePath);
        return 1;
    }

    // get the file URL
    char* url = file_readLine(fp);

    // print info
    fprintf(stdout, "score   %d doc   %d: %s\n", maxpair->max_count, maxpair->max_key, url);

    // clean
    fclose(fp);
    mem_free(url);
    mem_free(filePath);

    return 0;

}