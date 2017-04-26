// Parameter und Resultat werden ueber globale Variablen
// zwischen Threads transportiert

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

// Globale Daten fuer Thread-Kommunikation

long th_param;   // long Wert fuer initialen Parameter der Thread-Funktion
long th_result;  // long Wert fuer das Resultat des Threads

void *thread_func(void *arg) {
  printf("Thread entered; initial thread parameter is: %ld\n",*((long*)arg));
  th_result = 5;
  return (void*) &th_result;
}

int main() {
  pthread_t threadId;
  int status;

  long* p_th_result;  // Pointer auf globale Variable fuer Thread Resultat
  // Wird von Kernel bei pthread_join belegt.

  // Belege Parameter
  th_param = 10L;

  status = pthread_create(&threadId, NULL, thread_func, (void*)&th_param);
  if (status != 0) {
    exit(EXIT_FAILURE);
  }
  printf("Thread launched %ld\n", (unsigned long) threadId);
  pthread_join(threadId, (void **)&p_th_result);
  printf("Result is: %ld\n", *p_th_result);
  exit(EXIT_SUCCESS);
}
