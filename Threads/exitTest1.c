// What happens if we just call pthread_exit();
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  // We just call pthread exit in main()
  // Will the process be terminated ?
  printf("My PID is %d\n", (int) getpid());
  printf("Calling pthread_exit() in 5 seconds\n");
  sleep(5);
  pthread_exit(EXIT_SUCCESS);
}
