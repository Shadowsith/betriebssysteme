/*
   Example program for PHIL_NUM dining philosophers
   Solution with 1 mutex and PHIL_NUM unnamed counting semaphores.

   Since we use threads, unnamed semaphores are faster.
   In addition the handling of unnamed semaphores is easier if their number
   is unknown or large.
   For named sempahores we would have to introduce strings for their names.

   02.06.2012 FABR

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

#include "../Common/DijkstraSem.h"

#define COUNT 30
#define PHIL_NUM 9

// Unnamed semaphores and mutexes shared by all threads
sema *forks[PHIL_NUM];
sema *count_lock;

// Shared count protected by the mutex
int count = COUNT;

// Function for philosopher thread
void *phil_func(void *arg) {
  long n = (long) arg; // number of philosopher
  int fst, snd;     // number of fork semaphor

  while(true) {

    // Thinking
    printf("Philosopher %ld is thinking\n", n);

    // Get forks in ascending order
    if (n == PHIL_NUM) {
      // This is the philosopher with the highest number
      fst = 0;
      snd = PHIL_NUM-1;
    } else {
      fst = n-1;
      snd = n;
    }

    P(forks[fst]); 
    printf("Philosopher %ld is taking fork %d\n",n,fst);

    P(forks[snd]);
    printf("Philosopher %ld is taking fork %d\n",n,snd);

    // Check counter for maximal number of eating events
    P(count_lock);
    if ( count == 0) {
      // Release the lock and forks and exit
      V(count_lock);
      printf("Philosopher %ld exits since max count is reached \n", n);
      V(forks[snd]);
      printf("Philosopher %ld released fork %d\n",n,snd);
      V(forks[fst]);
      printf("Philosopher %ld released fork %d\n",n,fst);
      break; // leave while(1)
    }

    // Eating
    printf("Philosopher %ld is eating: count = %d\n", n, COUNT - count + 1);

    // Decrement counter for maximal number of eating events
    count--;

    // Release the lock and forks
    V(count_lock);

    V(forks[snd]);
    printf("Philosopher %ld released fork %d\n",n,snd);

    V(forks[fst]);
    printf("Philosopher %ld released fork %d\n",n,fst);

    usleep(rand() % 20);
  }

  return NULL;
}

int main() {
  pthread_t phils[PHIL_NUM];
  int status;
  long i;
  int j;

  // Check if long fits into void* variable
  assert(sizeof(long) <= sizeof(void *));

  // Create unnamed semaphores with initial value == 1
  if (NULL == ( count_lock = DijkstraSemCreateNN(1) ) ){
    printf("Main thread: unable to create semaphore COUNT_LOCK\n");
    exit(EXIT_FAILURE);
  }
  for (j=0; j < PHIL_NUM; j++) {
    if (NULL == ( forks[j] = DijkstraSemCreateNN(1) ) ){
      printf("Main thread: unable to create semaphore forks[%d]\n",j);
      exit(EXIT_FAILURE);
    }
  }
  printf("Main thread created semaphores\n");

  // Start philosopher threads
  for (i=1; i<=PHIL_NUM; i++) {
    status = pthread_create(&phils[i-1], NULL, phil_func, (void*) i);
    if (status != 0) {
      exit(EXIT_FAILURE);
    }
    printf("Thread for philosopher %ld launched\n", i);
  }

  // Wait for philosopher threads to finish
  for (i=1; i<=PHIL_NUM; i++) {
    pthread_join(phils[i-1], NULL);
  }

  // Cleanup handles
  if (!DijkstraSemCloseNN(count_lock) ) {
    exit(EXIT_FAILURE);
  }
  for (j=1; j<=PHIL_NUM; j++) {
    if (!DijkstraSemCloseNN(forks[j-1]) ) {
      exit(EXIT_FAILURE);
    }
  }

  exit(EXIT_SUCCESS);
}
