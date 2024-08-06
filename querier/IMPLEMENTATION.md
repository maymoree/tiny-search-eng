# CS50 TSE Querier
## Implementation Spec

Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We use two data structures: counters to hold docID and score of words, and index to hold the word, docID and how many times the word appeared in that docID.
We also have 2 local data structures: twoctrs to hold 2 counters that we will merge, and maxpair to hold the max value of the key and count.

## Control flow

The Querier is implemented in one file `querier.c`, with fourteen functions.
We will be going over the main functions (parseArg, takeQuery, sort_max)

### main

The `main` function simply calls `parseArg` and `takeQuery`, loads an index from `indexFilename`, then exits zero.

### parseArg

Given arguments from the command line, extract them into the function parameters; return only if successful.

* for `pageDirectory`, call `pagedir_validate()`
* for `indexFilename`, ensure that the file is readable
* if any trouble is found, print an error to stderr and exit non-zero.

### takeQuery

Do the real work of querying from `pageDirectory` and the index created from `indexFilename`.

Pseudocode:
	initialize pointer to query and prompt question
	read from stdin until EOF
		check if query is valid (contains alphabet and space only)
		parses query
		checks for correct syntax
        prints out the cleaned query
        check if query is an <orsequence>
        sorts and prints out matches
		cleans / free memory

### sort_max

This function implements sorts the output matched documents according to the score.
Given the final `counters`, the number of pairs inside the counters and the `pageDirectory`.

Pseudocode:
	for the amount of pairs inside the counter
        set max key and count to 0
        initialize a maxpair structure and set its key and count to 0
        iterate through the counter and find the max key and count; set it to the maxpair
        print the maxpair out in the correct format
        set the count of maxpair to 0
        clean / free

## Function prototypes

### querier

```c
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
```

## Error handling and recovery

Inputs were validated prior, according to the assumptions. 
The program exits with non-zero value if:
- out of memory.
- invalid command-line arguments.
- unable to read a file named pageDirectory/.crawler.
- unable to read a file named pageDirectory/1.
- unable to read a file named indexFilename.
Otherwise, the program reprompts the user.

## Testing plan

Testing is done in 'testing.sh', which pipelines commands into the querier. 'testing.sh' utilizes my own *crawler* and *indexer* output. The pageDirectory references letters-depth-2 and the indexFilename provides the index-letters-2.

'testing.sh' runs through a series of test cases, beginning with error test cases involving invalid command-line arguments and invalid query input. It then runs through a series of valid test cases, outputting matching documents in order of their score.

As mentioned in querier's README.md, the 'testing.sh' output is stored in 'testing.out', but the format of this file may seem a little strange as the querier program is intended for a user to submit stdin and thus prompting calls appear in testing output.