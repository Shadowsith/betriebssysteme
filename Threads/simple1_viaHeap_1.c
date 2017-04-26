// Parameter werden ueber Heap-Objekt an Thread uebergeben.
// Dieser uebergibt Resultate ebenfalls ueber Heap-Objekt.
// Bei Verwendung vieler Threads besser zu handhaben als globale Variablen.
//
// Speicherung von Parametern und Resultaten in getrennten Objekten macht
// Handhabung jedoch aufwaendig.
// Siehe simple1_viaHeap2.c fuer bessere Variante.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

// Vereinbarung der Struktur fuer Uebergabe Argumente
struct Args {
  int a;
  int b;
};

// Vereinbarung der Struktur fuer Uebergabe Resultat
struct Result {
  int div;
  int rest;
};

void *thread_func(void *arg) {
  struct Args* p_args; 
  struct Result* p_res; 

  p_args = arg;  // Impliziter Cast durch Zuweisung
  printf("Thread entered; initial thread parameters are: a=%d b=%d\n",
      p_args->a, p_args->b);

  // Speicher fuer Resultatobjekt allozieren
  if ( (p_res = malloc(sizeof(struct Result))) == NULL) {
    fprintf(stderr,"Out of memory\n");
    exit(EXIT_FAILURE);
  }
  // Division mit Rest durchfuehren
  // Resultate speichern in Resultobjekt
  p_res->div  = p_args->a / p_args->b;
  p_res->rest = p_args->a % p_args->b;

  // Speicher fuer Argumentobjekt freigeben
  free(p_args);

  // Zeiger auf Resultatobjekt als void* zurueckgeben
  return (void*) p_res;
}

int main() {
  pthread_t threadId;
  int status;

  struct Args* p_args;   // Zeiger auf Argumentobjekt im Heap
  struct Result* p_res;  // Zeiger auf Resultatobjekt im Heap

  // Speicher fuer Argumentobjekt allozieren
  if ( (p_args = malloc(sizeof(struct Args))) == NULL) {
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
  // pthread_t ist ein Typ (size_t xyz), aber von Architektur und OS ist das anders
  // deshalb davon ausgehen, dass pthread_t einfach ein fester Typ ist!
  printf("Thread launched %ld\n", (unsigned long) threadId);

  // Auf Thread warten; Adresse des Pointers p_res als Parameter uebergeben.
  // Damit kann Laufzeitsystem dort den Pointer auf Resultatobjekt hinterlegen.
  // Achtung: Argumentobjekt existiert nicht mehr, da schon in thread_func freigegeben
  pthread_join(threadId, (void **)&p_res);

  printf("Result is %d rest %d\n", p_res->div, p_res->rest);

  // Speicher fuer Resultatobjekt freigeben
  free(p_res);
  exit(EXIT_SUCCESS);
}
