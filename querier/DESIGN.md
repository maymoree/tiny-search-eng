# CS50 TSE Querier
## Design Spec

Here we focus on the core subset:

- User interface
- Inputs and outputs
- Functional decomposition into modules
- Pseudo code (plain English-like language) for logic/algorithmic flow
- Major data structures
- Testing plan

## User interface

The querier interacts with the user both when being called in the command-line as well as through stdin. In the command-line, the user is expected to include two additional arguments: the *pageDirectory* which holds the pages that were crawled by crawler, and the *indexFilename* which holds the index file produced by indexer. When the user runs the querier program with the appropriate arguments, they will be prompted to enter a query, thus allowing the user to type into stdin their search queries.

```bash
$ querier pageDirectory indexFilename
```

## Inputs and outputs

*Input:* There are command-line arguments as described above. Users will also be prompted to enter a query once the program runs. 

*Output:* Per the requirements spec, the querier outputs the following in order:
* a prompt (Query? )
* the query that was inputted (Query: )
* number of documents that matched the query
* the document's score, ID, and URL in order of score
It may also output any errors regarding inputs.

## Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which checks arguments and calls takeQuery 
 2. *parseArg*, which validates command-line arguments
 3. *takeQuery*, which validates (query syntax), queries, and sorts
 4. *parseQuery*, which helps rid query of white space and adds each word into an array
 5. *checkQuery*, which checks the syntax of the query
 6. *c_union*, which iterates through 2 counters in an orsequence
 7. *c_union_helper*, which helps c_union  
 8. *c_intersect*, which iterates over 2 counters in an andsequence
 9. *c_intersect_helper*, which helps c_intersect
 10. *count_counters*, which counts how many pairs are in a counters
 11. *count_matches*, which counts how many pairs with count more than 0 are in a counters
 12. *sort_max*, which sorts and prints out max pair and sets its count to 0
 13. *sort_max_helper*, which helps sort_max find the max
 14. *print_score*, which outputs the findings in a format

## Pseudo code for logic/algorithmic flow

The querier will run as follows:

1. Parse and validate command-line arguments
2. Initialize modules and variables (index)
3. Take queries from stdin until EOF
4. Parse and validate the query
5. Use the index to find matching documents
6. Rank the documents based on their scores
7. Print the list of documents

## Major data structures

Helper modules provide 2 data structures we need:

- *index* of words, the docIDs of the pages that hold the word, and the counts of times they appear.
- *counters* to hold the docIDs and scores of words

Local data structures/types we need:
- *twoctrs* to hold 2 counters that we need to merge
- *maxpair* to hold the key (docID) and count (score) of the pair with the max count

## Testing plan

Testing is done in 'testing.sh', which pipelines commands into the querier. 'testing.sh' utilizes my own *crawler* and *indexer* output. The pageDirectory references letters-depth-2 and the indexFilename provides the index-letters-2.

'testing.sh' runs through a series of test cases, beginning with error test cases involving invalid command-line arguments and invalid query input. It then runs through a series of valid test cases, outputting matching documents in order of their score.

As mentioned in querier's README.md, the 'testing.sh' output is stored in 'testing.out', but the format of this file may seem a little strange as the querier program is intended for a user to submit stdin and thus prompting calls appear in testing output.