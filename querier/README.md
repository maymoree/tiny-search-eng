****************
* EXTENSION USED
****************

# CS50 Querier (Lab 6)
## Memo Tangtipongkul (memo-t)

## Assumptions
- pageDirectory has files named 1, 2, 3, ..., without gaps.
- The content of files in pageDirectory follow the format as defined in the specs; no extensive error checking.
- The content of the file named by indexFilename follows our index file format (as defined in Lab 5); no extensive error checking.
- The provided index file corresponds to the provided pageDirectory, that is, was built by indexer from the files in that directory.

## Functionality
The program meets the full spec. That is, the querier supports 'and' precedence over 'or', and prints the document set in decreasing order by score.

## Files and Directory
- Makefile - compilation procedure
- querier.h - the interface
- querier.c - the implementation
- testing.sh - test file for valid and invalid cases (including valgrind test)
- testing.out - result of make test &> testing.out
### Used in Testing
- letters-2 - a directory container files produced by ./crawler
- letters-2.index - a file produced by ./indexer

## Compilation
To compile, simply `make`.

## Testing
- To test errors and valid cases, `make test`. See testing.out for details of testing and an example test run.
- Did not use `fuzzyquery.c` to test since arguments were pipped into testing.sh.
- Did not implement anything from the <unistd.h> since the output looks confusing the the testing.out.

