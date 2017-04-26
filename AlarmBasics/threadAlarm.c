// This version uses pthread_create to create a
// separate thread to wait for each alarm to expire.
//
// Original source:
// Programming with POSIX Threads
// David R. Butenhof
// ISBN 0-201-63392-2
//
// Minor changes by FABR
// 2012-04-16
//  - use common library function ngets()
//  - add include files
//  - changed error reporting
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "ngets.h"

#define BUFSIZE 64

struct alarm_tag {
  int         seconds;
  char        message[64];
};

void *thread_func (void *arg) {
  struct alarm_tag *alarm = (struct alarm_tag *) arg;
  int status;

  // Thread detaches itself
  status = pthread_detach (pthread_self ());
  if (status != 0) {
    fprintf(stderr, "Detach thread; status = %d", status);
    exit(EXIT_FAILURE); // Terminate entire program
  }

  sleep (alarm->seconds);
  printf ("(%d) %s\n", alarm->seconds, alarm->message);

  // Free allocated memory
  free (alarm);
  return NULL;
}

int main (int argc, char *argv[]) {
  int seconds;
  char line[BUFSIZE];
  char message[BUFSIZE];

  struct alarm_tag* palarm;
  pthread_t thread;
  int status;

  while (1) {
    printf ("New Alarm (seconds to wait and name) > ");
    if(! ngets(line, BUFSIZE) ) {
      printf("Error reading alarm from stdin\n");
      exit(EXIT_FAILURE);
    }

    if (strlen (line) == 0) {
      continue;
    }

    // Parse input line into seconds (%d) and a message
    // separated from the seconds by whitespace.
    if (sscanf (line, "%d %s", &seconds, message) < 2) {
      fprintf (stderr, "Bad input command\n");
      continue;
    }

    // Normal control path
    palarm = malloc (sizeof (struct alarm_tag));
    if (palarm == NULL) {
      perror("Unable to allocate memory for alarm");
      exit(EXIT_FAILURE);
    }

    // Load data for thread into alarm_tag structure
    palarm -> seconds = seconds;
    strcpy(palarm->message, message);

    // Create thread for this alarm
    status = pthread_create(&thread, NULL, thread_func, palarm);
    if (status != 0) {
      fprintf(stderr, "Error creating alarm thread");
      exit(EXIT_FAILURE);
    }

    // There is no reason to join with the alarm thread.
    // We detach the new thread inside the thread function.
  }
}
