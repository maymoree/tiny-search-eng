# Memo Tangtipongkul
# Fall 2023
# Testing crawler.c errors and with valgrind

#!/bin/bash

# make directory
mkdir test1

echo "----invalid usage -- not enough command-line arguments"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html test1
echo ""

echo "----invalid usage -- too many command-line arguments"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html test1 3 4
echo ""

echo "----external seedURL"
./crawler https://en.wikipedia.org/wiki/Science test1 3 
echo ""

echo "----invalid maxDepth"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html test1 11
echo ""

echo "----Unable to do pageDirectory/.crawler"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html test2 11
echo ""

echo "----Valid initial arguments BUT scans duplicate/external URLs as it crawls"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html test1 10
echo ""

# make directory
mkdir test2

echo "----Valgrind with toscrape at depth 1"
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html test2 1
echo ""