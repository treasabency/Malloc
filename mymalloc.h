#ifndef _mymalloc_H_
#define _mymalloc_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <math.h>

#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )
#define MYSIZE 4096

static char myblock[MYSIZE];

struct entry
{
	int isfree; //0 - not free
	int entrySize;
	struct entry *prev;
	struct entry *next;
};

static void *memEntries[MYSIZE/sizeof(struct entry) + 1] = {0};

void *mymalloc(size_t size, char* file, int linenum);
void myfree(void* p, char* file, int linenum);

#endif // _mymalloc_H_
