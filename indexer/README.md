# CS50 TSE: Indexer
## Student name: Memo Tangtipongkul
## Github Username: memo-t

## Implementation

There are three data structures, which include hashtables, sets, and counters. Both start empty. When processing a given document identified by docID, the program considers each word, looks up the word in the hashtable, and retrieves a pointer to a counters_t* item. Finally, it uses counters_add() on that item, using the docID as a key.

There are three functions within indexer.c.

The main function simply parses the arguments, calls indexBuild and indexPage, then exits zero. 
The indexBuild function creates a new 'index' object, loads a webpage from the document file, and passes the webpage and docID to indexPage.
The indexPage function steps through each word of the webpage, looks up the word in the index, and increments the count of occurrences of this word in this docID accordingly.


## Assumptions
No assumptions beyond those that are clear from the spec.

## Files
- Makefile - compilation procedure
- indexer.h - the interface of indexer
- indexer.c - the implementation of indexer
- indextest.c - the testing file for indexer
- testing.sh - test file
- testing.out - result of make test
- datafiles - .crawler directory from my crawler folder
- datafiles2 - .crawler directory from my crawler folder
- notcrawler - not a crawler directory
- read-only.index - file that can only read

## Testing

Type "make" to compile.
Type "make test" to test (with valgrind). 
See testing.out for details of testing.