/*
   Demonstrate unnamed semaphors for use in threads and processes
   with shared memory.

   Example program with four worker threads demonstrates producer/consumer
   problem and solution with two counting semaphors and a mutex.

   Since we use threads, unnamed semaphores are more convenient
   than named semaphores.
   Using POSIX condition variables would even be faster

   Note 1
   the mutex allows for multiple consumers/producers.
   If we restrict ourselves to only one producer and one consumer
   our special implementation of the ring buffer would not require
   a mutex to protect access to the buffer. A consumer and a producer
   can never interfere due to organization via indexes head, tail.

   May be optimized by using separate locks for producers and consumers

   Note 2
   Here, we use the trick, that the thread that invokes the P() operation
   is different from the one that invokes the V() operation on the semaphor!

   Producer performs P(thebuf.cntFree)    and V(thebuf.cntEntries)
   Consumer performs P(thebuf.cntEntries) and V(thebuf.cntFree);

   In contrast the P()/V() operations on the mutex thebuf.lock are always
   performed pairwise by the same thread!
   This demonstrates the difference between mutexes and binary semaphores.

   02.06.2012 FABR

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

#include "../Common/DijkstraSem.h"

#define COUNT 10
#define SLEEP 10
#define SIZE 7

// A global ring buffer structure

struct RingBuffer {
  int buf[SIZE];
  int head;
  int tail;

  // Shared semaphores and mutexes
  sema *cntFree;
  sema *cntEntries;
  sema *lock;
} thebuf;

// Producer functions
void storeProduct(int value) {
  int oldtail;

  P(thebuf.cntFree);
  printf("Producer %lu woke up from P(thebuf.cntFree)\n",
      (unsigned long) pthread_self());

  // Lock the buffer to guarantee mutual exclusion of producers
  P(thebuf.lock);
  printf("Producer %lu woke up from P(thebuf.lock)\n",
      (unsigned long) pthread_self());

  // Store
  oldtail = thebuf.tail;
  thebuf.buf[thebuf.tail] = value;
  thebuf.tail = (thebuf.tail + 1) % SIZE;

  printf("Producer %lu stored value %d at index %d\n",
      (unsigned long) pthread_self(), value, oldtail);

  printf("Producer %lu doing V(thebuf.lock)\n",
      (unsigned long) pthread_self());
  V(thebuf.lock);

  printf("Producer %lu doing V(thebuf.cntEntries)\n",
      (unsigned long) pthread_self());
  V(thebuf.cntEntries);
}

// Start function of producer threads
void *prod_func(void *arg) {
  long count = (long) arg;
  int i;
  int value;

  for (i=0; i < count; i++) {
    // Produce
    value = rand() % 100;

    printf("Producer %lu produced value %d\n",
        (unsigned long) pthread_self(), value);

    // Store the product
    storeProduct(value);

    // Sleep for at most SLEEP micro seconds
    usleep(rand() % SLEEP);
  }
  return NULL;
}

// Consumer  functions

// Get product for consumption
int getProduct(void) {
  int value;
  int oldhead;

  P(thebuf.cntEntries);
  printf("\tConsumer %lu woke up from P(thebuf.cntEntries)\n",
      (unsigned long) pthread_self());

  // Lock the buffer to guarantee mutual exclusion of consumers
  P(thebuf.lock);
  printf("\tConsumer %lu woke up from P(thebuf.lock)\n",
      (unsigned long) pthread_self());

  // Retrieve the product
  oldhead=thebuf.head;
  value = thebuf.buf[thebuf.head];
  thebuf.head = (thebuf.head + 1) % SIZE;

  printf("\tConsumer %lu fetched value %d from index %d\n",
      (unsigned long) pthread_self(), value, oldhead);

  printf("\tConsumer %lu doing V(thebuf.lock)\n",
      (unsigned long) pthread_self());
  V(thebuf.lock);

  printf("\tConsumer %lu doing V(thebuf.cntFree)\n",
      (unsigned long) pthread_self());
  V(thebuf.cntFree);

  return value;
}

// Start function of consumer threads
void *cons_func(void *arg) {
  long count = (long) arg;
  int i;
  int value;

  for (i=0; i < count; i++) {
    // Get the product
    value = getProduct();

    // Consume
    printf("\tConsumer %lu consumed value %d\n",
        (unsigned long) pthread_self(), value);

    // Sleep for at most SLEEP micro seconds
    usleep(rand() % SLEEP);
  }
  return NULL;
}

int main() {
  pthread_t prod1,prod2;
  pthread_t cons1,cons2;
  int status;

  // Initialize the ring buffer
  thebuf.head=0;
  thebuf.tail=0;

  // Initialize random generator
  srand ( time(NULL) );

  // Check if long fits into void* variable
  assert(sizeof(long) <= sizeof(void *));

  // Open semaphores and create them with initial values if needed
  if (NULL == ( thebuf.lock = DijkstraSemCreateNN(1) ) ){
    printf("Main thread: unable to create semaphore LOCK\n");
    exit(EXIT_FAILURE);
  }
  if (NULL == ( thebuf.cntFree = DijkstraSemCreateNN(SIZE) ) ){
    printf("Main thread: unable to create semaphore CNT_FREE\n");
    exit(EXIT_FAILURE);
  }
  if (NULL == ( thebuf.cntEntries = DijkstraSemCreateNN(0) ) ){
    printf("Main thread: unable to create semaphore CNT_ENTRIES\n");
    exit(EXIT_FAILURE);
  }

  printf("Main thread opened semaphores\n");

  // Start producer threads
  status = pthread_create(&prod1, NULL, prod_func, (void*) COUNT);
  if (status != 0) {
    exit(EXIT_FAILURE);
  }
  printf("Thread prod1 launched %lu\n", (unsigned long) prod1);

  status = pthread_create(&prod2, NULL, prod_func, (void*) COUNT);
  if (status != 0) {
    exit(EXIT_FAILURE);
  }
  printf("Thread prod2 launched %lu\n", (unsigned long) prod2);

  // Sleep for at most 200 micro seconds
  usleep(rand() % 200);

  // Start consumer threads
  status = pthread_create(&cons1, NULL, cons_func, (void*) COUNT);
  if (status != 0) {
    exit(EXIT_FAILURE);
  }
  printf("Thread cons1 launched %lu\n", (unsigned long) cons1);

  status = pthread_create(&cons2, NULL, cons_func, (void*) COUNT);
  if (status != 0) {
    exit(EXIT_FAILURE);
  }
  printf("Thread cons2 launched %lu\n", (unsigned long) cons2);


  // Wait for worker threads to complete
  pthread_join(prod1, NULL);
  pthread_join(prod2, NULL);
  pthread_join(cons1, NULL);
  pthread_join(cons2, NULL);

  // Cleanup handles
  if (!DijkstraSemCloseNN(thebuf.lock) ) {
    exit(EXIT_FAILURE);
  }
  if (!DijkstraSemCloseNN(thebuf.cntEntries) ) {
    exit(EXIT_FAILURE);
  }
  if (!DijkstraSemCloseNN(thebuf.cntFree) ) {
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
