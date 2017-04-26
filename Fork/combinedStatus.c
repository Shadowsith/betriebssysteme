// Show purpose of combined exit status
//
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main () {
  pid_t k, pid;
  int status;

  k=fork();
  if (k == 0) {
    printf ("Child with PID %d; may parent has PPDI %d\n",
        getpid(), getppid());
    //abort();
    exit(0xab);
    //exit(EXIT_SUCCESS);
  } else if (k < 0) {
    exit(EXIT_FAILURE);
  } else {
    printf ("Parent waiting for child's dead\n");
    pid=wait (&status);
    if (pid == -1) {
      exit(EXIT_FAILURE);
    } else {
      printf ("Parent: Child %d just died", pid);
      if (WIFEXITED(status)) {
        printf(" normally:\n\tcombined status 0x%x\n\tand exit code 0x%x.\n",
            status, WEXITSTATUS(status));
      } else {
        printf(", abnormal termination: var status = 0x%x.\n",status);
      }
      exit(EXIT_SUCCESS);
    }
  }
}
