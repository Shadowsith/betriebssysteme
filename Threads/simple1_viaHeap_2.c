// Parameter werden ueber Heap-Objekt an Thread uebergeben.
// Dieser uebergibt das Resultat ebenfalls ueber Heap-Objekt.
// Bei Verwendung vieler Threads besser zu handhaben als globale Variablen.
//
// Parameter und Resultate werden in einer gemeinsamen Struktur genutzt.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

// Vereinbarung der Struktur fuer Uebergabe Argumente
// und Resultate
struct ArgsResult {
  int a;
  int b;
  int div;
  int rest;
};

void *thread_func(void *arg) {
  struct ArgsResult* p_args; 

  p_args = arg;  // Impliziter Cast durch Zuweisung
  printf("Thread entered; initial thread parameters are: a=%d b=%d\n",
      p_args->a, p_args->b);

  // Division mit Rest durchfuehren
  // Resultate speichern in Resulttatobjekt
  p_args->div  = p_args->a / p_args->b;
  p_args->rest = p_args->a % p_args->b;

  // Zeiger auf Resultatobjekt als void* zurueckgeben
  return arg;
}

int main() {
  pthread_t threadId;
  int status;

  struct ArgsResult* p_args;   // Zeiger auf Objekt im Heap

  // Speicher fuer Argumentobjekt allozieren
  if ( (p_args = malloc(sizeof(struct ArgsResult))) == NULL) {
    fprintf(stderr,"Out of memory\n");
    exit(EXIT_FAILURE);
  }

  // Belege Argumentobjekt
  p_args->a = 30;
  p_args->b = 8;

  // Thread starten
  status = pthread_create(&threadId, NULL, thread_func, (void*)p_args);
  if (status != 0) {
    exit(EXIT_FAILURE);
  }
  printf("Thread launched %ld\n", (unsigned long) threadId);

  // Auf Thread warten;
  // Im vorliegenden Programm ist klar, dass die nach join im Pointer p_args
  // hinterlegte Adresse die gleiche ist, wie die bei Aufruf von pthread_create
  // uebergebene.
  // Die Uebergabe von (void **)&p_args wird dann interessant, wenn man
  // mehrere Threads startet. Ueber p_args kann man nach join immer einfach
  // auf das richtige Ergebnis zugreifen. Keine Verwaltung der Pointer
  // auf Resultatobjekte ueber threadId notwendig.
  pthread_join(threadId, (void **)&p_args);

  printf("Result von %d div %d is %d rest %d\n",
      p_args->a, p_args->b, p_args->div, p_args->rest);

  // Speicher fuer Objekt freigeben
  free(p_args);
  exit(EXIT_SUCCESS);
}

// Aufgabe:
// Programm erweitern um Schleife, die 5 Threads mit unterschiedlichen
// Argumenten startet.
// Hinweis: die IDs der erzeugten Threads muessen geeignet verwaltet
// werden, damit man auf alle Threads ein join ausfuehren kann.
