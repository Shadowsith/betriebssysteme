// Origin: AoC_codes_full/Chapter 6/Example 6-4
// The Art Of Concurrency: A Thread Monkey's Guide to Writing Parallel Applications
// Clay Breshers, ISBN 978-0-596-52153-0, O'Reilly, 2009
//
// 2012-04-13   Major rewrite by fabr
// - dynamic allocation of memory
// - encapsulation of data into structure ThreadData
// - timer code

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "nsectimer.h"

// ---------------------------------------------------------------
// GLOBALS
// ---------------------------------------------------------------

// Options with predefined values

// Number of threads to use for computing partial sums
int numThreads = 4;

// Size of the array containing the integers to be summed
//
// Using unsigned long on a 64bit machine (siezof(unsigned long) == 8)
// the following results in 4GB RAM for the array!
// 1024 * 1024 * 1024 * 4 / 8 = 536870912
//
// Using unsigned long on a 32bit machine (siezof(unsigned long) == 4)
// the following results in 1GB RAM for the array!
// 1024 * 1024 * 1024 * 1 / 4 = 268435456

unsigned long size = 536870912;
//unsigned long size = 268435456;

// Use debug mode
bool debug = false;

// Factor for busy waiting
int factor = 0;

// ---------------------------------------------------------------
// Type for passing data to and from threads
// ---------------------------------------------------------------

struct ThreadData {
  int tnum;               // The threads number
  unsigned long size;     // Size of the array
  int numThreads;         // Number of threads
  unsigned long *anArray; // Base pointer of array
  unsigned long long sum; // The partial sum
  int factor;             // A factor for busy waiting
};

// ---------------------------------------------------------------
// Function for busy waiting
// ---------------------------------------------------------------

// ---------------------------------------------------------------
// The thread's function for computing a partial sum
// ---------------------------------------------------------------
// Note: we use a single shared object pointed to by pArg to
// pass arguments to the thread function and for returning
// the result. The object of type struct ThreadData is
// allocated on the heap.

void *summation (void *pArg) {
  struct ThreadData *data = pArg; // Cast argument

  unsigned long long lSum = 0;
  long start, end, i;

  // Derive boundaries of section from data->tnum
  start = (data->size / data->numThreads) * data->tnum;
  end   = (data->size / data->numThreads) * (data->tnum + 1);
  if (data->tnum == data->numThreads - 1){
    end = data->size;
  }

  // Compute the partial sum
  for (i = start; i < end; i++) {
    lSum += (data->anArray)[i];
  }

  // Do a busy wait depending on data->factor
  // Note: from the absolut factor for the busy wait we compute
  // one that depends on the maximal number of threads
  float x=0.0;
  for (i = 0; i < (data->factor / data->numThreads * 1000000 ); i++) {
    x+=92.77/29388; 
  }

  // Store partial sum in thread's data structure
  data->sum = lSum;

  // Return pointer to thread's data structure as result
  return pArg;
}

// ---------------------------------------------------------------
// Auxiliary function
// ---------------------------------------------------------------

// Fill the array with values
void initializeArray(unsigned long *anarray, unsigned long size) {
  unsigned long i;
  for (i = 0; i < size; i++){
    anarray[i] = i+1;
  }
}

// Print the usage information
void usage(char* prog) {
  printf("Usage: %s [-d] [-f factor] [-s array_size] [-t num_threads]\n", prog);
}

// Read command line options
int readCommandLineOptions(int argc, char* argv[]) {
  char c;

  while((c = getopt(argc, argv, "df:s:t:")) != -1) {
    switch(c) {
      case('d'):
        debug = true;
        continue;
      case('f'):
        factor = atoi(optarg);
        continue;
      case('s'):
        size = atol(optarg);
        continue;
      case('t'):
        numThreads = atoi(optarg);
        continue;
      default:
        usage(argv[0]);
        return -1;
    }
  }
  // Skip all options processed
  argc -= optind;
  argv += optind;

  // Are there any non-option arguments left?
  // In our case no further argument is allowed
  if (argc > 0 ) {
    usage(argv[0]);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

// ---------------------------------------------------------------
// The MAIN function
// ---------------------------------------------------------------

int main(int argc, char* argv[]) {
  int j;
  unsigned long long  sum = 0;      // The overall sum
  pthread_t* pHandles;              // Pointer to array for storing the handles to all threads

  // Pointers to data shared by threads
  unsigned long *pTheArray;         // Pointer to array of values to sum up
  struct ThreadData *pThreadData;   // Structure for passing data to and from threads

  // Some timers:
  struct nsectimer* tmalloc = createTimer("Malloc");
  struct nsectimer* tinitArray = createTimer("InitArray");
  struct nsectimer* tsummation = createTimer("Summation");

  // Read command line options
  if(readCommandLineOptions(argc, argv)){
    exit(EXIT_FAILURE);
  }

  // Allocate space for the array of thread handles
  pHandles = malloc (sizeof(pthread_t) * numThreads);
  if (pHandles == NULL) {
    fprintf(stderr, "Unable to allocate memory for array of thread handles\n");
    // We depend on the OS for freeing allocated memory during exit
    exit(EXIT_FAILURE);
  }

  // Allocate space for the array of numbers to sum up
  startTimer(tmalloc);
  pTheArray = malloc (sizeof(unsigned long) * size);
  if (pTheArray == NULL) {
    fprintf(stderr, "Unable to allocate memory for array of numbers\n");
    // We depend on the OS for freeing allocated memory during exit
    exit(EXIT_FAILURE);
  }
  endTimer(tmalloc);

  startTimer(tinitArray);
  initializeArray(pTheArray,size);  // Initialize the array
  endTimer(tinitArray);

  startTimer(tsummation);
  for (j = 0; j < numThreads; j++) {
    // Allocate the thread's data structure
    pThreadData = malloc(sizeof (struct ThreadData));
    if (pThreadData == NULL) {
      fprintf(stderr, "Unable to allocate memory for thread %d\n", j);
      // We depend on the OS for freeing allocated memory during exit
      exit(EXIT_FAILURE);
    }
    // Fill in data
    pThreadData-> tnum = j;
    pThreadData-> size = size;
    pThreadData-> numThreads = numThreads;
    pThreadData-> anArray = pTheArray;
    pThreadData-> sum = 0;
    pThreadData-> factor = factor;

    // Launch thread
    // The threads derive the boundaries of their sections
    // from pThreadData-> tnum
    if (pthread_create(&pHandles[j], NULL, summation, (void *)pThreadData)){
      fprintf(stderr, "Unable to start thread %d\n", j);
      // We depend on the OS for freeing allocated memory during exit
      exit(EXIT_FAILURE);
    }
  }

  // Collect partial sums from all threads
  for (j = 0; j < numThreads; j++) {
    if (pthread_join(pHandles[j], (void**) &pThreadData)) {
      fprintf(stderr, "Unable to join with thread %d\n", j);
      // We depend on the OS for freeing allocated memory during exit
      exit(EXIT_FAILURE);
    }
    sum += pThreadData->sum;
    if(debug) {
      printf("Partial sum of thread %d is %Lu\n", j, pThreadData->sum);
    }
    free(pThreadData);
  }
  endTimer(tsummation);

  // Display all timers with nanosecond values
  if(debug) {
    printf("Size of array %lu byte\n", size * sizeof(unsigned long));
    printf("Sum is %Lu\n",sum);
    printf("Number of threads %d\n", numThreads);
    displayTimer(tmalloc);
    displayTimer(tinitArray);
    displayTimer(tsummation);
  } else {
    // Display timer for summation in double format for stats
    printf("%6.3f\n", getTimer(tsummation));
  }

  // Free allocated memory
  free(pHandles);
  free(pTheArray);

  free(tmalloc);
  free(tinitArray);
  free(tsummation);

  // Check result via Gauss'ian formula
  unsigned long long ull_size = size;
  unsigned long long check_size = ull_size * (ull_size + 1) / 2;
  if ( check_size != sum) {
    fprintf(stderr,"Wrong sum for array size == %Lu\n",ull_size);
    fprintf(stderr,"Expected %Lu got %Lu\n",check_size, sum);
    return EXIT_FAILURE;
  } else {
    return EXIT_SUCCESS;
  }
}
