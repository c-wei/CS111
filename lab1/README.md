## UID: 306220886

## Pipe Up

Low-level C program to replicate the pipe operator in shell by manipulating file descriptors.

## Building
Make sure you're in the directory of your pipe.c and Makefile and run the command:

make

## Running

cs111@cs111 CS111/labi (main %) » ./pipe ls cat wc
7 7 63

cs111@cs111 CS111/labl (main %) » ls | cat | wc
7 7 63

## Cleaning up

Run the command: 

make clean