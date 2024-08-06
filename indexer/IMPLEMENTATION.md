# CS50 TSE Indexer
# Name: Memo Tangtipongkul
# Github Username: memo-t
## Implementation Spec

In this document we reference the [Requirements Specification](https://github.com/CS50Fall2023/home/blob/main/labs/tse/indexer/REQUIREMENTS.md) and [Design Specification](https://github.com/CS50Fall2023/home/blob/main/labs/tse/indexer/DESIGN.md) and focus on the implementation-specific decisions.

The TSE indexer is a program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. 
Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We use three data structures, which include hashtables, sets, and counters. When processing a given document identified by docID, the program considers each word, looks up the word in the hashtable, and retrieves a pointer to a counters_t* item. Finally, it uses counters_add() on that item, using the docID as a key.

The index maps from words to (docID, count) pairs, representing the number of occurrences of that word in that document. A hashtable is used wherein each item is a counters_t where the docID is used as a key. 

When building the inverted index, it's impossible to know in advance how many words you will find in all those pages encountered by the crawler. I have set the number of slots to 200. 

## Control flow

The Indexer is implemented in one file `indexer.c`, with three function: main, indexBuild, and indexPage.

### main

The `main` function parses arguments and initializes other modules.

### indexBuild

The `indexBuild` function builds an in-memory index from webpage files it finds in the pageDirectory.

### indexPage

The `indexPage` function scans a webpage document to add its words to the index.

## Other modules

### pagedir

`pagedir` is a module that provides functions to load webpages from files in the pageDirectory.

We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about how to initialize and validate a pageDirectory, and how to write and read page files, in one place.

### index

`index` is a module providing the data structure to represent the in-memory index, and functions to read and write index files.

### webpage

`webpage` is a module providing the data structure to represent webpages, and to scan a webpage for words.

### word

`word` is a module providing a function to normalize a word.

## Function prototypes

### indexer

```c
int main(const int argc, char* argv[]);
index_t* indexBuild(char* pageDirectory);
void indexPage(index_t* index, webpage_t* page, int docID);
```

### pagedir

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_load(const char* pageDirectory, const int docID);
```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Certain errors are caught and handled internally.

## Testing plan

Here is an implementation-specific testing plan.

The script `testing.sh` that invokes the indexer several times, with a variety of command-line arguments.

The file `indextest.c` can also be used to test.