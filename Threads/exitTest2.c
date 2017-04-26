// Was passiert, wenn wir den Haupt-Thread mittels pthread_exit() beenden
// und einen anderen Thread vorher gestartet haben?

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

void *thread_func(void *arg) {
  pthread_detach(pthread_self());
  printf("Thread entered; sleeping: %ld\n",(long)arg);
  sleep((long) arg);

  return NULL;
}

int main() {
  pthread_t threadId;
  long a=10;
  int status;

  printf("Main thread: PID = %d\n",(int) getpid());

  // Pruefung, ob long in void* Platz hat
  assert(sizeof(long) <= sizeof(void *));

  status = pthread_create(&threadId, NULL, thread_func, (void*)a);
  if (status != 0) {
    exit(EXIT_FAILURE);
  }
  printf("Thread launched %ld\n", (unsigned long) threadId);

  pthread_detach(pthread_self());
  printf("Main thread calls pthread_detach()\n");
  pthread_exit(EXIT_SUCCESS);
}
