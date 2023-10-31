/* Запускаются 1000 процесов  и каждый процес инкрементирует глобальную 
 * переменную на единицу и получается кализия */
#include <stdio.h>
#include <pthread.h>
#define SIZE 1000

long Sum = 0;

void* threadCall(void* argc)
{
  int *Ptr = (int*) argc;
  
  for((*Ptr) = 0; (*Ptr) < SIZE; (*Ptr)++)
    Sum++;
  return NULL;
}

int main (char* argc, char** argv)
{
  int *s;
  int index;
  int index1[SIZE];
  
  pthread_t thread[SIZE];

  for(index = 0; index < SIZE; index++ ){
    index1[index] = 0;
    pthread_create(&thread[index], NULL, threadCall, (void*) &index1[index]);
  }

  for(index = 0; index < SIZE; index++){
    pthread_join(thread[index], (void**) &s);
  }

  printf("Sum = %lu\n\n", Sum);

  return 0;
}
