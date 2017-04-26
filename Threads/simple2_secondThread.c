// Frage: kann man einen Thread aus einem Thread heraus starten?
// Antwort: natuerlich, beliebig viele sogar!
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

void *thread_func2(void *arg) {
  printf("\t\tSecondary thread entered\n");
  printf("\t\tInitial thread parameter is: %ld\n",(long)arg);
  return (void*) 3;
}

void *thread_func(void *arg) {
  pthread_t threadId;
  long result;
  int status;

  printf("\tThread entered\n");
  printf("\tInitial thread parameter is: %ld\n",(long)arg);
  printf("\tLaunching secondary thread\n");

  status = pthread_create(&threadId, NULL, thread_func2, arg);
  if (status != 0) {
    // Do not call exit() !!!
    pthread_exit((void*) -1);
  }
  printf("\tThread launched %ld\n", (unsigned long) threadId);
  pthread_join(threadId, (void **)&result);
  printf("\tResult of secondary thread is: %ld\n", result);
  return (void*) 17 + result;
}

int main() {
  pthread_t threadId;
  long a=10, result;
  int status;

  // Pruefung, ob long in void* Platz hat
  assert(sizeof(long) <= sizeof(void *));

  status = pthread_create(&threadId, NULL, thread_func, (void*)a);
  if (status != 0) {
    exit(EXIT_FAILURE);
  }
  printf("Thread launched %ld\n", (unsigned long) threadId);
  pthread_join(threadId, (void **)&result);
  printf("Result is: %ld\n", result);
  exit(EXIT_SUCCESS);
}
