/*
   Dijkstra Semaphore test program
   Two processes synchronize on a named semaphore
   Process A waits for process B but not vice versa

   Two processes in different files
   Process A

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

  if (argc == 2 && strcmp(argv[1],"nocreate")==0) {
    // Do not create semaphore if not existent, just try to open
    if (NULL == ( sync = DijkstraSemOpen(SEMNAME) ) ){
      printf("A: unable to open semaphore\n");
      exit(EXIT_FAILURE);
    }
    printf("A opened semaphore\n");
  } else {
    // Open the semaphore and create it with initial value if needed
    if (NULL == ( sync = DijkstraSemCreate(SEMNAME,0) ) ){
      printf("A: unable to create semaphore\n");
      exit(EXIT_FAILURE);
    }
    printf("A created/opened semaphore\n");
  }

  i=0;
  while (true) {
    P(sync);
    i++;
    printf("A returned from P operation (iteration %d)\n",i);
  }

  /* Won't get here */
  exit(EXIT_SUCCESS);
}
