# Makefile for the "crawler" program.
#
# Memo Tangtipongkul, Fall 2023

L = ../libcs50
S = ../common

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$S
OBJS = crawler.o
LIBS = -lm
LLIBS = $L/libcs50-given.a 
SLIBS = $S/common.a

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

.PHONY: all clean

all: crawler

# executable depends on object files
crawler: $(OBJS) $(LLIBS) $(SLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(SLIBS) $(LIBS) -o crawler

# object files depend on include files
crawler.o: crawler.h

# includes test for errors, working, and valgrind
test: 
	-bash ./testing.sh &> testing.out

# includes last test with all the big files
bigtest:
	-bash -v ./bigtesting.sh &> bigtesting.out

# VALGRIND is checked in testing.sh
# valgrind: crawler
# 	$(VALGRIND) ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html bomb1 10

clean:
	rm -f crawler
	rm -f *~ *.o
	rm -rf *.dSYM