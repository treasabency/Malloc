#ifndef _mymalloc_H_
#define _mymalloc_H_

#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )
static char myblock[4096];
unsigned int goodMallocs;
unsigned int badMallocs;
unsigned int goodFrees;
unsigned int badFrees;
unsigned int outsideFrees;
unsigned int redundantFrees;
void sizeSet(unsigned int index, unsigned int num);
unsigned int sizeRead(unsigned int index);
void* mymalloc(size_t size, char* file, int linenum);
void myfree(void* p, char* file, int linenum);

#endif /* _mymalloc_H_ */
