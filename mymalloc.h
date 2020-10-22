#ifndef _mymalloc_H_
#define _mymalloc_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <math.h>

#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )

static char myblock[];

void *mymalloc(unsigned size, char* file, int linenum);
void myfree(void* p, char* file, int linenum);

#endif /* _mymalloc_H_ */