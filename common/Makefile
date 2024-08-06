# Makefile for pagedir module
#   Builds and compiles all these utility functions into a library.
#
# Memo Tangtipongkul, Fall 2023

# object files, and the target library
OBJS = pagedir.o word.o index.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

# Build $(LIB) by archiving object files
# (you will need to drop in copy of set.c, counters.c, hashtable.c)
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h ../libcs50/webpage.h ../libcs50/hashtable.h ../libcs50/file.h
word.o: word.h 
index.o: index.h ../libcs50/webpage.h ../libcs50/hashtable.h ../libcs50/file.h ../libcs50/counters.h 

.PHONY: clean sourcelist

# list all the sources and docs in this directory.
# (this rule is used only by the Professor in preparing the starter kit)
sourcelist: Makefile *.md *.c *.h
	@echo $^

# clean up after our compilation
clean:
	rm -rf *.dSYM
	rm -f core
	rm -f $(LIB) *~ *.o
