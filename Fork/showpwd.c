//  Umgebungsvariable PWD abrufen

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char *sptr;

    sptr = getenv("PWD");
    if (sptr == NULL) {
        // Achtung: errno wird nicht gesetzt durch errno
        // Daher keine Verwendung von perror()
        fprintf (stderr, "Fehler bei getenv(\"PWD\")\n");
        exit(EXIT_FAILURE);
    }
    printf("My current work directory is %s\n", sptr);
    exit(EXIT_SUCCESS);
}
