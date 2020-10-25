#include "mymalloc.h"

//malloc, free, malloc, free... 120 cycles
void A(void* p)
{
	int i;
	for(i = 0; i < 120; i++){
		p = malloc(1);
		free(p);
	}
}

//malloc() 1 byte 120 times, then free() 120 times
void B(void* p, int i)
{
	if(i == 120)
		return;

	else{
		p = malloc(1);
		i++;
		void* q = NULL;
		B(q,i);
		free(p);
	}
}

//randomly, either malloc 1 byte or attempt free(), 120 times
void CHelper(void *p, int m, int f)
{
	if(f == 120){
		return;
	}
	else if(m == 120){
		f++;
		free(p);
		void* q = NULL;
		CHelper(q, m, f);
	}

	else{
		if(f<m){
			int r = rand() % 2;
			if(r){
				p = malloc(1);
				m++;
				void* q = NULL;
				CHelper(q, m, f);
			}
			else{
				f++;
				free(p);
				void* q = NULL;
				CHelper(q, m, f);
			}
		}
		else{
			p = malloc(1);
			m++;
			void* q = NULL;
			CHelper(q, m, f);
		}
	}
}

void C()
{
	void* p = NULL;
	CHelper(p, 0, 0);
}

//randomly decides to either malloc or free, but for random sizes
void DHelper(void *p, int m, int f)
{
	if(f == 120){
		return;
	}
	else if(m == 120){
		f++;
		free(p);
		void* q = NULL;
		DHelper(q, m, f);
	}
	else{
		if(f<m){
			int r1 = rand()%2;
			if(r1){
				int randSize = rand()%101;
				p = malloc(randSize);
				m++;
				void* q = NULL;
				DHelper(q, m, f);
			}
			else{
				f++;
				free(p);
				void* q = NULL;
				DHelper(q, m, f);
			}
		}
		else{
			int randSize = rand()%101;
			p = malloc(randSize);
			m++;
			void* q = NULL;
			DHelper(q, m, f);
		}
	}
}

void D()
{
	void* p = NULL;
	DHelper(p, 0, 0);
}

//increasing equal numbers of mallocs and frees up to 100 of each
void E(void* p)
{
	int i;
	for(i = 0; i < 10; i++){
		for(int j = 0; j < i; j++){
			p = malloc(1);
			free(p);
		}
	}
}

//run each workload 50 times, output mean runtimes to console
int main(){
	int i;
	unsigned int timeA, timeB, timeC, timeD, timeE, timeAll;
	timeA = timeB = timeC = timeD = timeE = timeAll = 0;

	void* p = NULL;
	struct timeval start;
	struct timeval end;
	struct timeval fullStart;
	struct timeval fullEnd;
	srand(time(NULL));

	gettimeofday(&fullStart, 0);

	for(i = 0; i < 50; i++)
	{
		gettimeofday(&start, 0);
		A(p);
		gettimeofday(&end, 0);
		timeA += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
	}

	for(i = 0; i < 50; i++)
	{
		gettimeofday(&start, 0);
		B(p, 1);
		gettimeofday(&end, 0);
		timeB += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
	}

	for(i = 0; i < 50; i++)
	{
		gettimeofday(&start, 0);
		C();
		gettimeofday(&end, 0);
		timeC += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
	}
	
	for(i = 0; i < 50; i++)
	{
		gettimeofday(&start, 0);
		D();
		gettimeofday(&end, 0);
		timeD += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
	}

	for(i = 0; i < 50; i++)
	{
		gettimeofday(&start, 0);
		E(p);
		gettimeofday(&end, 0);
		timeE += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
	}

	gettimeofday(&fullEnd, 0);
	timeAll += (fullEnd.tv_sec - fullStart.tv_sec)*1000000 + fullEnd.tv_usec - fullStart.tv_usec;

	printf("\n---------------------------------------------\n");
	printf("Average time for workload A: %d ms\n", timeA/100);
	printf("Average time for workload B: %d ms\n", timeB/100);
	printf("Average time for workload C: %d ms\n", timeC/100);
	printf("Average time for workload D: %d ms\n", timeD/100);
	printf("Average time for workload E: %d ms\n", timeE/100);
	printf("Average time for a full A-E workload: %d ms\n", timeAll/100);
	printf("---------------------------------------------\n\n");

	return 0;
}
