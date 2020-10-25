#include "mymalloc.h"

//malloc, free, malloc, free... 120 cycles
void runA(void* ptr)
{
	for(int i = 0; i < 120; i++) {
		ptr = malloc(1);
		free(ptr);
	}
}

//malloc() 1 byte 120 times, then free() 120 times
void runB(void* ptr, int level)
{
	if(level == 120)
		return;

	else {
		ptr = malloc(1);
		level++;

		void* newPtr = NULL;
		runB(newPtr, level);
		//free all pointers after recursion ends
		free(ptr);
	}
}

//randomly, either malloc 1 byte or attempt free(), 120 times
void CRec(void *ptr, int mallocs, int frees)
{
	if(frees == 120) {
		return;
	}

	else if(mallocs == 120) {
		free(ptr);
		frees++;
		void* newPtr = NULL;
		CRec(newPtr, mallocs, frees);
	}

	else 
	{
		if(frees < mallocs) 
		{
			int choice = rand() % 2;

			if(choice) {
				ptr = malloc(1);
				mallocs++;
				void* newPtr = NULL;
				CRec(newPtr, mallocs, frees);
			}
			else {
				free(ptr);
				frees++;
				void* newPtr = NULL;
				CRec(newPtr, mallocs, frees);
			}
		}
		//cannot free if no allocated memory
		else {
			ptr = malloc(1);
			mallocs++;
			void* newPtr = NULL;
			CRec(newPtr, mallocs, frees);
		}
	}
}

//run with recursive helper
void runC(void* ptr)
{
	CRec(ptr, 0, 0);
}

//recursively runs i mallocs and i frees (numOps, or operations)
void DRec(void* ptr, int numOps, int level)
{
    if(level == numOps)
        return;

    ptr = malloc(1);
    void* newPtr = NULL;
    level++;
    DRec(newPtr, numOps, level);
    free(ptr);
}

//increasing powers of two groups of mallocs, then frees, up to 32 of each
void runD(void* ptr)
{
    for(int i = 1; i <= pow(2, 5); i *= 2)
        DRec(ptr, i, 0);
}

//increasing equal numbers of mallocs and frees up to 100 of each
void runE(void* ptr)
{
	for(int i = 0; i < 100; i++) {
		for(int j = 0; j < i; j++) {
			ptr = malloc(1);
			free(ptr);
		}
	}
}

//run each workload 50 times, output mean runtimes to console
int main()
{
	unsigned int i;
	unsigned int timeA, timeB, timeC, timeD, timeE, timeAll;
	timeA = timeB = timeC = timeD = timeE = timeAll = 0;

	void* ptr = NULL;
	struct timeval start;
	struct timeval end;
	struct timeval fullStart;
	struct timeval fullEnd;
	srand(time(NULL));

	gettimeofday(&fullStart, 0);

	//add difference in microseconds each time, divide by 1000 later for milliseconds
	for(i = 0; i < 50; i++)
	{
		gettimeofday(&start, 0);
		runA(ptr);
		gettimeofday(&end, 0);
		timeA += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
	}

	for(i = 0; i < 50; i++)
	{
		gettimeofday(&start, 0);
		runB(ptr, 1);
		gettimeofday(&end, 0);
		timeB += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
	}

	for(i = 0; i < 50; i++)
	{
		gettimeofday(&start, 0);
		runC(ptr);
		gettimeofday(&end, 0);
		timeC += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
	}
	
	for(i = 0; i < 50; i++)
	{
		gettimeofday(&start, 0);
		runD(ptr);
		gettimeofday(&end, 0);
		timeD += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
	}

	for(i = 0; i < 50; i++)
	{
		gettimeofday(&start, 0);
		runE(ptr);
		gettimeofday(&end, 0);
		timeE += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
	}

	gettimeofday(&fullEnd, 0);
	timeAll += (fullEnd.tv_sec - fullStart.tv_sec)*1000000 + fullEnd.tv_usec - fullStart.tv_usec;

	printf("\n---------------------------------------------\n");
	printf("Average time for workload A: %d ms\n", timeA/1000);
	printf("Average time for workload B: %d ms\n", timeB/1000);
	printf("Average time for workload C: %d ms\n", timeC/1000);
	printf("Average time for workload D: %d ms\n", timeD/1000);
	printf("Average time for workload E: %d ms\n", timeE/1000);
	printf("Average time for a full A-E workload: %d ms\n", timeAll/1000);
	printf("---------------------------------------------\n\n");

	return 0;
}