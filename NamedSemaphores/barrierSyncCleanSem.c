/*
   Dijkstra Semaphore test program
   Two processes synchronize on a named semaphore

   Two processes in different files
   This program removes the named semaphore from the system

   01.06.2012  by FABR

   Derived from:
   Simple Semaphore test program
   AUP2, Sec. 7.09.2, 7.10.1
   */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../Common/DijkstraSem.h"

#define SEMNAME "/DijkstraSem_SYNC"

int main(void) {

  // Mark instance of semaphore for removal
  // Will be removed by system as soon as no references are left
  if ( !DijkstraSemRemove(SEMNAME) ) {
    exit(EXIT_FAILURE);
  }

  printf("Removed named semaphore %s\n", SEMNAME);

  exit(EXIT_SUCCESS);
}
