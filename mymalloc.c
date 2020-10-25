#include "mymalloc.h"

static char myblock[MAXSIZE];
//pointer to entries of myblock
static void *blockPtr[MAXSIZE/sizeof(struct entry) + 1] = {0};

// search for the first possible unoccupied entry in myblock
int firstFree()
{
	int entriesSize = MAXSIZE/sizeof(struct entry) + 1;
	for(int i = 0; i < entriesSize; i++)
		if(blockPtr[i] == 0) 
			return i;
  	return 1;
}

//attempt to allocate memory of given size within myblock
void *mymalloc(size_t allocSize, char* file, int line)
{
	int init = 0;
	struct entry *curr, *next, *head;

	if (allocSize == 0) {
		printf("ERROR: Could not allocate zero bytes (FILE: %s, LINE: %d)\n", file, line);
		return 0;
	}

	//head is uninitialized, set to start of myblock
	if(!init)
	{
		head = (struct entry*) myblock;

		head->entrySize = MAXSIZE - sizeof(struct entry);
		head->isFree = 1;
		head->prev = head->next = 0;

		init = 1;
		blockPtr[firstFree()] = myblock;
	}

	curr = head;
	while(curr)
	{
		if(curr->entrySize < allocSize || !(curr->isFree)) {
			//not enough space, or taken, move on
			curr = curr->next;
		}
		else if(curr->entrySize < (allocSize + sizeof(struct entry))) {
	  	//current segment has memory, but next one does not have enough for
	  	//header, so do not initialize header for next space
			curr->isFree = 0;
			return (char*)curr + sizeof(struct entry);
		}
		else {
	  		//reinitialize to header for next memory space
			next = (struct entry*)((char*)curr + sizeof(struct entry) + allocSize);
			next->prev = curr;
			next->next = curr->next;
			next->entrySize = curr->entrySize - sizeof(struct entry) - allocSize;
			next->isFree = 1;

			blockPtr[firstFree()] = next;
			curr->entrySize = allocSize;
			curr->isFree = 0;
			curr->next = next;
			return (char*)curr + sizeof(struct entry);
		}
	}

	//curr is null, meaning out of bounds
	printf("ERROR: Not enough memory space available (%ld bytes attempted) (FILE: %s, LINE: %d)\n", allocSize, file, line);
	return 0;
}

//attempt to free the memory pointed to by toFree
void myfree(void *toFree, char *file, int line)
{
	struct entry *ptr, *prev, *next;
	int entriesSize = MAXSIZE/sizeof(struct entry) + 1;

	if (toFree == NULL) {
		printf("ERROR: Pointer is NULL-valued, free failed (FILE: %s, LINE: %d)\n", file, line);
		return;
	}

	//convert toFree to entry* pointer
	ptr = (struct entry*)((char*)toFree - sizeof(struct entry));

	//make sure pointer has been allocated previously
	int i, valid = 0;
	for (i = 0; i < entriesSize; i++) {
		if (ptr == blockPtr[i] && !(ptr->isFree)) {
		  valid = 1;
		  break;
		}
	}

	if (!valid) {
		printf("ERROR: Cannot free unallocated memory space (FILE: %s, LINE: %d)\n", file, line);
		return;
	}

	//previous memory space and current one may be merged
	if((prev = ptr->prev) != 0 && prev->isFree)
	{
		prev->entrySize += sizeof(struct entry) + ptr->entrySize;
		//remove the free entry
		blockPtr[i] = 0;
	}

	//set as free space
	else
	{
		ptr->isFree = 1;
		prev = ptr;
	}

	//next memory space and current one may be merged
	if((next = ptr->next) != 0 && next->isFree)
	{
		prev->entrySize += sizeof(struct entry) + next->entrySize;
		//subsume current entry
		prev->next = next->next;

		//removing further free entries within memory segment
		for (i = 0; i < entriesSize; i++) {
			if (next == blockPtr[i]) {
				blockPtr[i] = 0;
				break;
			}
		}
	}
}