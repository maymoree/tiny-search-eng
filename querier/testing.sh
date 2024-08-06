# Memo Tangtipongkul
# Fall 2023
# Testing querier.c

#!/bin/bash

pagedir="./letters-2"
indexfile="./letters-2.index"

# Invalid Test Cases
# Testing Error Cases 
# ----------------------------------------- 

# Error Test Case 1: no arguments"
./querier
# -----------------------------------------

# "Error Test Case 2: invalid pagedir"
./querier invalid $indexfile
# ----------------------------------------- 

# "Error Test Case 3: invalid indexFilename"
./querier $pagedir invalid
# -----------------------------------------

# "Error Test Case 4: blank query"
./querier $pagedir $indexfile <<< "        "
# -----------------------------------------

# "Error Test Case 5: invalid operators"
./querier $pagedir $indexfile <<< "and for or first"
# -----------------------------------------

# "Error Test Case 6: invalid operators"
./querier $pagedir $indexfile <<< "for and or first"
# -----------------------------------------

# "Error Test Case 7: invalid operators"
./querier $pagedir $indexfile <<< " for or first and   "
# -----------------------------------------

# "Error Test Case 8: invalid query (bad chars)"
./querier $pagedir $indexfile <<< "    home or 293847928!!!   "
# -----------------------------------------

# "Error Test Case 9: nonexistent word in <andsequence>"
./querier $pagedir $indexfile <<< "memo and computer"
# -----------------------------------------

# "Testing Valid Cases"
# -----------------------------------------

# "Valid Test Case 1: single word"
./querier $pagedir $indexfile <<< "home"
# -----------------------------------------

# "Valid Test Case 2: nonexistent word in <orsequence>"
./querier $pagedir $indexfile <<< "for or memo"
# -----------------------------------------

# "Valid Test Case 3: <andsequence> with valid words"
./querier $pagedir $indexfile <<< "home and this"
# -----------------------------------------

# "Valid Test Case 4: sequence of valid words (with valgrind)"
valgrind --leak-check=full --show-leak-kinds=all ./querier $pagedir $indexfile <<< "home this or search algorithm"
# -----------------------------------------

exit 0