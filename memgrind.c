#include "mymalloc.h"

//malloc() 1 byte and immediately free it 120 times in a row
void A(void* p){
  int i;
  for(i = 0; i < 120; i++){
    p = malloc(1);
    free(p);
  }
}

//malloc() 1 byte 120 times, then free() the 120 byte chunks one by one
void B(void* p, int i){
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

//randomly, either malloc arr[i] or free arr[i-1]
void CHelper(char* arr[], int m, int f, int i){
  if(f == 120){
    return;
  }
  else if(m == 120){
    f++;
    free(arr[i-1]);
    i--;
    CHelper(arr, m, f, i);
  }
  
  else{
    if(f<m){
      int r = rand() % 2;
      if(r){
        arr[i]=malloc(1);
        i++;
        m++;
        CHelper(arr, m, f, i);
      }
      else{
        f++;
        free(arr[i-1]);
        i--;
        CHelper(arr, m, f, i);
      }
    }
    else{
      arr[i]=malloc(1);
      i++;
      m++;
      CHelper(arr, m, f, i);
    }
  }
}

//randomly choose between a 1 byte malloc() or free() 240 times
void C(){
  char* arr[120];
  CHelper(arr, 0, 0, 0);
}

//randomly decides to either malloc arr[i] or free arr[i-1]
void DHelper(char* arr[], int m, int f, int i){
  if(f == 120){
    return;
  }
  else if(m == 120){
    f++;
    free(arr[i-1]);
    i--;
    DHelper(arr, m, f, i);
  }
  else{
    if(f<m){
      int r1 = rand() % 2;
      if(r1){
        int r2 = rand()%501;
        arr[i]=malloc(r2);
        i++;
        m++;
        DHelper(arr, m, f, i);
      }
      else{
        f++;
        free(arr[i-1]);
        i--;
        DHelper(arr, m, f, i);
      }
    }
    else{
      int r2 = rand()%501;
      arr[i]=malloc(r2);
      i++;
      m++;
      DHelper(arr, m, f, i);
    }
  }
}

//randomly choose between a randomly-sized malloc() or free() 240 times
void D(){
  char* arr[120];
  DHelper(arr, 0, 0, 0);
}

//Does i mallocs followed by i frees
void EHelper(void* p, int i, int j){
  if(j==i){
    return;
  }
  p = malloc(1);
  void* q = NULL;
  j++;
  EHelper(q, i, j);
  free(p);
}

//increasing equal numbers of mallocs and frees up to 100 of each
void E(void* p){
  int i;
  for(i = 0; i < 100; i++){
    EHelper(p, i, 0);
  }
}

int main(){
  int i;
  unsigned int timeA, timeB, timeC, timeD, timeE, timeAll;
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
    gettimeofday(&end, NULL);
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

  gettimeofday(&fullEnd, NULL);
  timeAll += (fullEnd.tv_sec - fullStart.tv_sec)*1000000 + fullEnd.tv_usec - fullStart.tv_usec;

  printf("Average time for workload A: %d ms\n", timeA/100);
  printf("Average time for workload B: %d ms\n", timeB/100);
  printf("Average time for workload C: %d ms\n", timeC/100);
  printf("Average time for workload D: %d ms\n", timeD/100);
  printf("Average time for workload E: %d ms\n", timeE/100);
  printf("Average time for a full A-E workload: %d ms\n", timeAll);

  return 0;
}