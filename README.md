# cop5725-implementation-project

Implementation in C++ of the algorithms described in the 2016 ICDE paper "A Novel, Low-latency Algorithm for Multiple Group-By Query Optimization".

For each .table file, the naming convention is (name of table)(number of rows).table, wherein "k" is shorthand for thousand and "m" for million.

Naive solution with the following Group By's:

{"ORDERKEY"},
{"PARTKEY"},
{"ORDERKEY", "PARTKEY"}

Ran for 2219.46 seconds, or ~37 minutes. Long time could be because the 3rd group is the primary key for the table and/or we use vectors to store, and it takes O(n) time to search them.

# To Do

1. 'optimized' solution works similarly to naive but executes based on tree structure from (3)
3. algorithms from paper that create an optimal tree
    - algorithm 1
    - algorithm 6
    

# Tentative Plan

1. by thursday november 14
    - algorithm 1, 6
    - status report
    - presentation slides
    
2. by thursday november 21
    - optimal solution done
    - algorithm 2, 3 done
    - algorithm essentially complete
    
3. by tuesday november 26
    - testing and performance eval
    - write final report
    
4. by tuesday, dec 3
    - final report due / presentation

    
