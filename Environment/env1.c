//  Umgebungsvariablenliste ausgeben
//
// 01.03.12/fabr:
//  - fehlende include-Anweisungen eingefuegt

#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main() {
    int i;

    for (i=0; environ[i] != NULL; i++) {
        printf("%s\n", environ[i]);
    }
    exit(0);
}
