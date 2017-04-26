// Parameter und Resultat werden direkt per Wert transportiert.
// Dazu werden jeweilige void-Pointer als Container missbraucht!
// Dadurch kann man Verwendung globaler Variablen oder Heap-Objekte
// einsparen.
//
// Wertung:
// Boeses aber haeufig eingesetztes Programmier-Muster, falls jeweils nur ein
// Parameter-Wert und ein Resultat-Wert ausgetauscht werden muessen, die zudem
// in einer Zeiger-Variablen Platz finden.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

void *thread_func(void *arg) {
  printf("Thread entered; initial thread parameter is: %ld\n",(long)arg);
  return (void*) 5;
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
