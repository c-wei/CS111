# Hash Hash Hash

C implementation of thread safe hash table using mutexes to prevent race conditions.

## Building
Run the following command in the lab3 directory:
```shell
make
```

## Running
run:
```shell
./hash-table-tester -t num_threads -s num_entries
```
replace num_threads and num_entries with your number of threads and entries respectively.

## First Implementation
In the `hash_table_v1_add_entry` function, I added a long around the hash table in the add_entry function since this is where the race condition occurs.

### Performance
Run
```shell
./hash-table-tester -t 4 -s 50000
```
I got the Results:
```shell
Generation: 45,071 usec
Hash table base: 219,352 usec
  - 0 missing
Hash table v1: 530,250 usec
  - 0 missing
Generation: 45,598 usec
Hash table base: 238,244 usec
  - 0 missing
Hash table v1: 465,615 usec
  - 0 missing
Generation: 44,566 usec
Hash table base: 219,189 usec
  - 0 missing
Hash table v1: 550,873 usec
  - 0 missing
```
Version 1 is a little slower/faster than the base version since threads compete to lock the hash table which creates a bottleneck as only one thread is allowed in the critical section at a time.

## Second Implementation
In the `hash_table_v2_add_entry` function, I added a lock for each hash table entry since race conditions happen specifically when hash table entries are added to the same bucket simultaneously.

### Performance
Run
I got the Results:
```shell
Generation: 45,071 usec
Hash table base: 219,352 usec
  - 0 missing
Hash table v2: 141,046 usec
  - 0 missing
Generation: 45,598 usec
Hash table base: 238,244 usec
  - 0 missing
Hash table v2: 139,322 usec
  - 0 missing
Generation: 44,566 usec
Hash table base: 219,189 usec
  - 0 missing
Hash table v2: 102,097 usec
  - 0 missing
```

I got a 2 times speed up since there's an increase in parallelism with the v2 implementation and more threads can work in critical sections simultaneously and thus reducing bottlenecking but allows mutual exclusion through locking so race conditions don't occur.

## Cleaning up

Run:
```shell
make clean
```