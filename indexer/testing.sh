# Memo Tangtipongkul
# Fall 2023
# Testing indexer.c and indextest.c

#!/bin/bash

cd ../indexer
mkdir datafiles
mkdir datafiles2
cd ../crawler
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../indexer/datafiles 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../indexer/datafiles2 3
cd ../indexer
mkdir notcrawler

# ------------------------------------------------
# ------- invalid test cases for indexer.c -------
# ------------------------------------------------

# ------- no arguments
./indexer

# ------- too many arguments
./indexer ./datafiles ./testoutput.index fourth

# ------- invalid pageDirectory (non-existent)
./indexer noDirectory ./testoutput.index

# ------- invalid pageDirectory (not crawler)
./indexer  notcrawler ./testoutput.index

# ------- invalid indexFileName (invalid path)
./indexer ./datafiles ../notHere/gone.index


# ------- invalid indexFileName (read only)
touch read-only.index
chmod 444 read-only.index
./indexer ./datafiles read-only.index


# ------------------------------------------------
# ------- valid test cases for indexer.c -------
# ------------------------------------------------

# ------- test normally
./indexer ./datafiles test1.index

# ------- test with valgrind
valgrind --leak-check=full --show-leak-kinds=all ./indexer ./datafiles2 test2.index




# ------------------------------------------------
# ------- invalid test cases for indextest.c -------
# ------------------------------------------------

# ------- 0 arguments
./indextest

# ------- too many arguments
./indextest test1.index test1new.index fourth


# ------------------------------------------------
# ------- valid test cases for indextest.c.c -------
# ------------------------------------------------

# ------- test1 and test1new
valgrind --leak-check=full --show-leak-kinds=all ./indextest test1.index test1new.index

# ------- test2 and test2new
valgrind --leak-check=full --show-leak-kinds=all ./indextest test2.index test2new.index


# ------------------------------------------------
# ------- comparison for indextest.c.c -------
# ------------------------------------------------

# ------- for test1 should be 0
diff --brief <(sort test1.index) <(sort test1new.index) >/dev/null
echo $?

# ------- for test2 should be 0
diff --brief <(sort test2.index) <(sort test2new.index) >/dev/null
echo $?