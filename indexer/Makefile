# Makefile for the "indexer" program.
#
# Memo Tangtipongkul, Fall 2023

OBJS = indexer.o
OBJS2 = indextest.o
LIBS = ../common/common.a ../libcs50/libcs50.a

FLAGS = 
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

indexer: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

indextest: $(OBJS2) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

indexer.o: indexer.h

indextest.o: indextest.c

.PHONY: all test clean

all: indexer indextest

test: indexer indextest
	-bash -v ./testing.sh &> testing.out

clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f indexer
	rm -f indextest
	rm -f core