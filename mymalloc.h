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
#define MAXSIZE 4096

struct entry
{
	int isFree; //0 - not free
	int entrySize;
	struct entry *prev;
	struct entry *next;
};

int getFreeIndex();
void *mymalloc(size_t size, char* file, int linenum);
void myfree(void* p, char* file, int linenum);

#endif // _mymalloc_H_