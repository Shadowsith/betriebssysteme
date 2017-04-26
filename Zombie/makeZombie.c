// We create a temporary zombie process

// Usage: makeZombie
//
// Preparation:
// Inspect the process tree of the calling shell
//
// pstree -s -p $$
// init(1)───mate-terminal(3629)───bash(3850)───pstree(4319)
//
// Pick the process of the mate-terminal (here 3629) and execute
//
// pstree -s -p 3629 -H $$
// init(1)───mate-terminal(3629)─┬─bash(3635)───vi(3841)───{vi}(3842)
//                               ├─bash(3850)───pstree(4306)
//                               ├─bash(3979)
//                               ├─gnome-pty-helpe(3634)
//                               ├─{mate-terminal}(3633)
//                               └─{mate-terminal}(3636)


#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SLEEP_PARENT 20  // Seconds to sleep in parent
#define SLEEP_CHILD   7  // Seconds to sleep in child

// Fork a graphical terminal with a running top process
// that shows process status of parent and child.
//
// Special for mate-terminal:
// The forked process of the mate-terminal dies immediately and the terminal
// is executed as a thread of some super mate-terminal process.
// The 'top' process executed via -e becomes child of the super mate-terminal
// $ pstree -p -H $$ 3629
// mate-terminal(3629)─┬─bash(3635)───vi(3841)───{vi}(3842)
//                     ├─bash(3850)───pstree(4116)
//                     ├─bash(3979)───makeZombie(4110)───makeZombie(4111)
//                     ├─gnome-pty-helpe(3634)
//                     ├─top(4115)
//                     ├─{mate-terminal}(3633)
//                     └─{mate-terminal}(3636)
// 
// Therefore, we call waitpid immediately after the fork of the mate-terminal

int fork_top(int parent_pid, int child_pid){
  pid_t term_pid = fork();
  if (term_pid < 0){
    fprintf (stderr, "Error on fork of terminal: %s\n", strerror(errno));
    return(-1); // Return error code to caller in main
  } else if (term_pid == 0) {
    // In child process for terminal
    // Create a string for the top command
    char top_cmd[30];  // Process ids are at most 5 digits long
    sprintf(top_cmd,"top -p %d -p %d", parent_pid, child_pid);

    // Start terminal
    // Adapt command and parameters to your system
    execl("/usr/bin/mate-terminal",
        "mate-terminal", "--geometry=80x10", "--zoom=1.3", "-e", top_cmd,
        NULL);

    // We should never get here
    fprintf (stderr, "Error on execl for top: %s\n", strerror(errno));
    exit(EXIT_FAILURE); // Exit the forked process
  } else {
    int status;
    // In forking parent process.
    // Immediately reap exit status of forked process for mate-terminal
    waitpid(term_pid, &status,0);
    if (WIFEXITED(status)){
      printf("Parent: terminal process died normally with status: %d\n",
          WEXITSTATUS(status));
      return 0;
    } else {
      fprintf(stderr, "Parent: terminal process died unexpectedly with status: %d\n",
          WEXITSTATUS(status));
      return -1;
    }
  }
}

int main () {
  pid_t child, pid;
  int status;

  child=fork();
  if (child < 0) {
    // Error
    fprintf (stderr, "Error on fork of child: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  } else if (child == 0) {
    // In child
    printf("Child: my PID is %d\n", getpid());
    // sleep
    sleep(SLEEP_CHILD);
    printf("Child: Now, I will become a zombie: bye\n");
    exit(EXIT_SUCCESS);
  } else {
    // Parent
    // Fork a terminal with a running top command that shows parent and child
    if (fork_top(getpid(),child) != 0) {
      fprintf(stderr, "Parent: Error on fork of terminal\n");
    } 

    printf("Parent: ignoring my child %d for %d seconds\n",child,SLEEP_PARENT);
    printf("Parent: my PID is %d\n", getpid());

    sleep(SLEEP_PARENT);

    printf ("Parent: now I am reaping the exit status of my dead child\n");
    pid=waitpid(child, &status,0);
    if (pid == -1) {
      fprintf(stderr, "Parent: Error on wait for child: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
  }
  printf ("Parent: Done\n");
  exit(EXIT_SUCCESS);
}
