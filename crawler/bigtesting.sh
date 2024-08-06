# Memo Tangtipongkul
# Fall 2023
# Testing crawler.c with 3 big files

#!/bin/bash

mkdir letters-0
# ----Letters -- Depth 0 
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html letters-0 0

mkdir letters-1
# ----Letters -- Depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html letters-1 1

mkdir letters-2
# ----Letters -- Depth 2"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html letters-2 2

mkdir letters-10
# ----Letters -- Depth 10"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html letters-10 10


mkdir toscrape-0
# ----ToScrape -- Depth 0"
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html toscrape-0 0

mkdir toscrape-1
# ----ToScrape -- Depth 1"
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html toscrape-1 1

mkdir toscrape-2
# ----ToScrape -- Depth 2"
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html toscrape-2 2

mkdir toscrape-3
# ----ToScrape -- Depth 3"
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html toscrape-3 3


mkdir wiki-0
# ----Wikipedia -- Depth 0"
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html wiki-0 0

mkdir wiki-1
# ----Wikipedia -- Depth 1"
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html wiki-1 1

mkdir wiki-2
# ----Wikipedia -- Depth 2"
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html wiki-2 2