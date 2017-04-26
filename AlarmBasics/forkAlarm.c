// This version uses fork to create a new process to
// wait for each alarm to expire.
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
#include <sys/wait.h>

#include "ngets.h"

#define BUFSIZE 64

int main (int argc, char *argv[]) {
  int seconds;
  char line[BUFSIZE];
  char message[BUFSIZE];

  pid_t pid;

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
    pid = fork ();
    if (pid == -1) {
      perror("Fork");
      exit(EXIT_FAILURE);
    } else if (pid ==  0) {
      // If we're in the child, wait and then print a message
      sleep (seconds);
      printf ("(%d) %s\n", seconds, message);
      exit (EXIT_SUCCESS);
    } else {
      // In the parent, call waitpid() to collect any children that
      // have already terminated. Use non-blocking option WNOHANG
      do {
        pid = waitpid ((pid_t)-1, NULL, WNOHANG);
        if (pid == -1) {
          perror("Wait for child");
          exit(EXIT_FAILURE);
        }
      } while (pid != 0);
    }
  }
}
