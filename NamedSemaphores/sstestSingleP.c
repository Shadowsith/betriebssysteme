/*
   Dijkstra Semaphore test program
   Two processes synchronize asymmetrically on a named semaphore.
   Parent and child process in one file

   01.06.2012  by FABR

   Derived from:
   Simple Semaphore test program
   AUP2, Sec. 7.09.2, 7.10.1
   */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "../Common/DijkstraSem.h"

#define SEMNAME "/DijkstraSemTest"

int main(void) {
  sema *sem;   // holds a mark if parent reached the barrier
  pid_t pid;
  int i;

  if ((pid = fork()) == 0) {
    // Open the semaphore and create it with initial value if needed
    if (NULL == (sem = DijkstraSemCreate(SEMNAME,0) ) ){
      printf("Child: unable to create semaphore\n");
      exit(EXIT_FAILURE);
    }
    printf("Child opened semaphore\n");

    while (true) {
      P(sem);
      i++;
      printf("Child returned from P operation (iteration %d)\n",i);
    }
    /* Normally, won't get here */
    if (!DijkstraSemClose(sem) ) {
      exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
  }

  sleep(1);

  // Open the semaphore and create it with initial value if needed
  if (NULL == ( sem = DijkstraSemCreate(SEMNAME,0) ) ){
    printf("Parent: unable to create semaphore\n");
    exit(EXIT_FAILURE);
  }
  printf("Parent opened semaphore\n");

  for (i = 1; i <= 10; i++) {
    printf("Parent doing V operation (iteration %d)\n", i);
    V(sem);
    sleep(1);
  }

  // Terminate child process
  if ( -1 == kill(pid, SIGINT) ) {
    exit(EXIT_FAILURE);
  }

  // Wait for child to exit; discard status
  wait(NULL);

  // Cleanup handle
  if (!DijkstraSemClose(sem) ) {
    exit(EXIT_FAILURE);
  }
  // Mark instance of semaphore for removal
  // Will be removed by system as soon as no references are left
  if ( !DijkstraSemRemove(SEMNAME) ) {
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
