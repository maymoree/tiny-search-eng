# Makefile for the "querier" program.
#
# Memo Tangtipongkul, Fall 2023

OBJS = querier.o
LIBS = ../common/common.a ../libcs50/libcs50.a

FLAGS = 
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

querier: $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

querier.o: querier.h

.PHONY: all test clean

all: querier

test: querier
	-bash -v ./testing.sh &> testing.out

valgrind: querier
	$(VALGRIND) ./querier ./letters-2 ./letters-2.index


clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f querier
	rm -f core