# CS50 Crawler (Lab 4)
## Memo Tangtipongkul (memo-t)

## Implementation
We use two data structures: a 'bag' of pages that need to be crawled, and a 'hashtable' of URLs that we have seen during our crawl. Both start empty. The size of the hashtable (slots) is impossible to determine in advance.

The Crawler is implemented in one file crawler.c, with four functions. The main function simply calls parseArgs and crawl, then exits zero.The parseArgs function extracts arguments provided in the commandline into the function parameters. The crawl function does the real work of crawling from seedURL to maxDepth and saving pages in pageDirectory. Given a webpage, the pageScan function scans to extract any links (URLs), ignoring non-internal URLs; for any URL not already seen before (i.e., not in the hashtable), add the URL to both the hashtable pages_seen and to the bag pages_to_crawl. 

## Assumptions
No assumptions beyond those that are clear from the spec.

## Files
Makefile - compilation procedure
crawler.h - the interface
crawler.c - the implementation
testing.sh - test file for errors and simple files, including valgrind test
bigtesting.sh - lest category of test for big files
testing.out - result of make test &> testing.out
bigtesting.out - result of make bigtest &> bigtesting.out

## Compilation
To compile, simply make.

## Testing

To test errors, small files AND valgrind, `make test`. See testing.out for details of testing and an example test run.

To test bigger files, `make bigtest`. See bigtesting.out for details of testing and an example test run.