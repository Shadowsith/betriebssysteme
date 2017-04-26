/*
   Example program for 3 dining philosophers
   Solution with 1 mutex and 3 named semaphores.

   Since we use threads, unnamed semaphores would be faster.
   In addition named semaphores are quite awkward to use for 
   larger numbers of philosophers since we need to generate
   names for the named semaphores.

   02.06.2012 FABR

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

#include "../Common/DijkstraSem.h"

#define SEM_FORK1 "/Fork1"
#define SEM_FORK2 "/Fork2"
#define SEM_FORK3 "/Fork3"
#define SEM_COUNT_LOCK  "/COUNT_LOCK"

#define COUNT 20
#define PHIL_NUM 3

// Shared semaphores and mutexes
sema *forks[PHIL_NUM];
sema *count_lock;

// Shared count
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
      break;
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

  // Check if long fits into void* variable
  assert(sizeof(long) <= sizeof(void *));

  // Mark instances of semaphores for removal
  // Will be removed by system as soon as no references are left
  if ( !DijkstraSemRemove(SEM_FORK1) ) {
    exit(EXIT_FAILURE);
  }
  if ( !DijkstraSemRemove(SEM_FORK2) ) {
    exit(EXIT_FAILURE);
  }
  if ( !DijkstraSemRemove(SEM_FORK3) ) {
    exit(EXIT_FAILURE);
  }

  // Open semaphores and create them with initial values if needed
  if (NULL == ( count_lock = DijkstraSemCreate(SEM_COUNT_LOCK,1) ) ){
    printf("Main thread: unable to create semaphore SEM_COUNT_LOCK\n");
    exit(EXIT_FAILURE);
  }
  if (NULL == ( forks[0] = DijkstraSemCreate(SEM_FORK1,1) ) ){
    printf("Main thread: unable to create semaphore SEM_FORK1\n");
    exit(EXIT_FAILURE);
  }
  if (NULL == ( forks[1] = DijkstraSemCreate(SEM_FORK2,1) ) ){
    printf("Main thread: unable to create semaphore SEM_FORK2\n");
    exit(EXIT_FAILURE);
  }
  if (NULL == ( forks[2] = DijkstraSemCreate(SEM_FORK3,1) ) ){
    printf("Main thread: unable to create semaphore SEM_FORK3\n");
    exit(EXIT_FAILURE);
  }

  printf("Main thread opened semaphores\n");

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
  if (!DijkstraSemClose(count_lock) ) {
    exit(EXIT_FAILURE);
  }
  for (i=1; i<=PHIL_NUM; i++) {
    if (!DijkstraSemClose(forks[i-1]) ) {
      exit(EXIT_FAILURE);
    }
  }

  // Mark instances of semaphores for removal
  // Will be removed by system as soon as no references are left
  if ( !DijkstraSemRemove(SEM_FORK1) ) {
    exit(EXIT_FAILURE);
  }
  if ( !DijkstraSemRemove(SEM_FORK2) ) {
    exit(EXIT_FAILURE);
  }
  if ( !DijkstraSemRemove(SEM_FORK3) ) {
    exit(EXIT_FAILURE);
  }
  if ( !DijkstraSemRemove(SEM_COUNT_LOCK) ) {
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
