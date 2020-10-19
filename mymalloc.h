#include<stdio.h>
#include<stddef.h>

char myblock[4096]; // memory array

// contains metadat for allocation and deallocation
struct block{
 size_t size;
 int free;
 struct block *next; 
};

// initial allocation of memory, initially void
struct block *freeList= (void*)myblock;

// functions in the malloc.c file
void initialize();
void split(struct block *fitting_slot,size_t size);
void *MyMalloc(size_t noOfBytes);
void merge();
void myfree(void* ptr);
