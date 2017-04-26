#include <stdio.h>
#include <stdlib.h>

// Konstanten fuer die Begrenzung der Segmente
extern char  etext, edata, end; 

// Eine initialisierte globale Variable
int i=1;

// Eine uninitialisierte globale Variable
int g;

int main () {
  // Eine lokale Variable
  int n=2;

  // Initialisiere g
  g = 3;

  // Eine im Heap allozierte Variable
  int *pv = malloc(sizeof(int));
  *pv = 4;

  printf("Speicheradressen:\n");
  printf("                         Adress main() %12p\n", &main);
  printf("             Ende Code Segment (etext) %12p\n", &etext);
  printf("                             Adresse i %12p\n", &i);
  printf("     Ende initialisierte Daten (edata) %12p\n", &edata);
  printf("                             Adresse g %12p\n", &g);
  printf("     Ende uninitialisierte Daten (end) %12p\n", &end);
  printf("           Adresse von Integer im Heap %12p\n", pv);
  printf("                             Adresse n %12p\n", &n);

  return 0;
}
