#include "mymalloc.h"

// search for the first possible unoccupied entry in myblock
int getFreeIndex() 
{
	int entriesSize = MYSIZE/sizeof(struct entry) + 1;
	int i;

	for(i = 0; i < entriesSize; i++)
		if(memEntries[i] == 0) 
			return i;
  return 1; //should never reach here but 0 is always set as head
}

//allocate memory of given size within myblock
void *mymalloc(size_t allocSize, char* file, int line)
{
	int init = 0;
	struct entry *curr;
	struct entry *next;
	struct entry *head;
	int entriesSize = MYSIZE/sizeof(struct entry) + 1;

	if (allocSize== 0) {
		printf("ERROR: Could not allocate zero bytes (FILE: %s, LINE: %d)\n", file, line);
		return 0;
	}

	if(!init)
	{
	//set head to start of myblock
		head = (struct entry*) myblock;

		head->entrySize = MYSIZE - sizeof(struct entry);
		head->isfree = 1;
		head->prev = head->next = 0;

		init = 1;
		memEntries[getFreeIndex()] = myblock;
	}

	curr = head;
	do
	{
		if(curr->entrySize < allocSize || !curr->isfree) {
	  // the current chunk is smaller, go to the next chunk
	  // or this chunk was taken, go to the next
			curr = curr->next;
		}
		else if(curr->entrySize < (allocSize + sizeof(struct entry))) {
	  // this chunk is free and large enough to hold data, 
	  // but there's not enough memory to hold the HEADER of the next chunk
	  // don't create any more chunks after this one
			curr->isfree = 0;
			return (char*)curr + sizeof(struct entry);
		}
		else {
	  // take the needed memory and create the header of the next chunk
			next = (struct entry*)((char*)curr + sizeof(struct entry) + allocSize);
			next->prev = curr;
			next->next = curr->next;
			next->entrySize = curr->entrySize - sizeof(struct entry) - allocSize;
			next->isfree = 1;
			memEntries[getFreeIndex()] = next;
			curr->entrySize = allocSize;
			curr->isfree = 0;
			curr->next = next;
			return (char*)curr + sizeof(struct entry);
		}
	} while (curr);

	printf("ERROR: Not enough memory space available (%ld bytes attempted) (FILE: %s, LINE: %d)\n", allocSize, file, line);
	return 0;
}

// free a memory buffer pointed to by p
void myfree(void *toFree, char *file, int line)
{
	struct entry *ptr;
	struct entry *prev;
	struct entry *next;
	int entriesSize = MYSIZE/sizeof(struct entry) + 1;

	if (toFree == NULL) {
		printf("ERROR: Pointer is NULL-valued, free failed (FILE: %s, LINE: %d)\n", file, line);
		return;
	}

	ptr = (struct entry*)((char*)toFree - sizeof(struct entry));

	//check if valid entry ptr
	int i;
	int valid = 0;
	for (i = 0; i < entriesSize; i++) {
		if (ptr == memEntries[i] && !ptr->isfree) {
		  valid = 1; //entry is valid
		  break;
		}
	}

	if (!valid) {
		printf("ERROR: Cannot free unallocated memory space (FILE: %s, LINE: %d)\n", file, line);
		return;
	}

	if((prev = ptr->prev) != 0 && prev->isfree)
	{
		// the previous chunk is free, so
		// merge this chunk with the previous chunk
		prev->entrySize += sizeof(struct entry) + ptr->entrySize;
		memEntries[i] = 0; //merged with previous, so removing free entry
	}
	else
	{ //not setting entry to null b/c not necessarily removing it, just isfree=1
		ptr->isfree = 1;
		prev = ptr; // used for the step below
	}
	if((next = ptr->next) != 0 && next->isfree)
	{
		// the next chunk is free, merge with it
		prev->entrySize += sizeof(struct entry) + next->entrySize;
		prev->next = next->next;
		//prev->isfree = 1;
		for (i = 0; i < entriesSize; i++) {
			if (next == memEntries[i]) {
				memEntries[i] = 0; //merged with next, so removing free entry
				break;
			}
		}
	}
}
