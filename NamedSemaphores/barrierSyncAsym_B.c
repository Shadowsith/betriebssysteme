/*
   Dijkstra Semaphore test program
   Two processes synchronize on a named semaphore
   Process A waits for process B but not vice versa

   Two processes in different files
   Process B

   01.06.2012  by FABR

   Derived from:
   Simple Semaphore test program
   AUP2, Sec. 7.09.2, 7.10.1
   */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../Common/DijkstraSem.h"

#define SEMNAME "/DijkstraSem_SYNC"

int main(int argc, char** argv) {
  sema *sync; // holds a mark if B reached the barrier
  int i;

  // Open the semaphore and create it with initial value if needed
  if (NULL == ( sync = DijkstraSemCreate(SEMNAME,0) ) ){
    printf("B: unable to create semaphore\n");
    exit(EXIT_FAILURE);
  }

  printf("B created/opened semaphore\n");

  for (i = 1; i <= 10; i++) {
    printf("B doing V operation (iteration %d)\n", i);
    V(sync);
    sleep(1);
  }

  // Cleanup handle
  if (!DijkstraSemClose(sync) ) {
    exit(EXIT_FAILURE);
  }

  if (argc == 2 && strcmp(argv[1],"nocleanup")==0) {
    // We do not cleanup the semaphore
    // NOP
  } else {
    // Mark instance of semaphore for removal
    // Will be removed by system as soon as no references are left
    if ( !DijkstraSemRemove(SEMNAME) ) {
      exit(EXIT_FAILURE);
    }
  }
  exit(EXIT_SUCCESS);
}
