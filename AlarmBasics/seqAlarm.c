// Simple synchronous alarm program
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

#include "ngets.h"  // use save variant of gets()

#define BUFSIZE 64

int main (int argc, char *argv[]) {
  int seconds;
  char line[BUFSIZE];
  char message[BUFSIZE];

  while (1) {
    printf ("New Alarm (seconds to wait and name) > ");
    if(! ngets(line, BUFSIZE) ) {
      printf("Error reading alarm from stdin\n");
      exit(EXIT_FAILURE);
    }

    if (strlen(line) == 0 ) {
      continue;
    }

    // Parse input line into seconds (%d) and a message
    // separated from the seconds by whitespace.
    if (sscanf (line, "%d %s", &seconds, message) < 2) {
      fprintf (stderr, "Bad input command\n");
      continue;
    }

    // Normal control path
    sleep (seconds);
    printf ("(%d) %s\n", seconds, message);
  }
}

