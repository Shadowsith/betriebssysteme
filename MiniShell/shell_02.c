// shell_02.c
// Minimale shell mit Fehlerbehandlung
//
// Abbruch der Endlosschleife mit Ctrl-C oder leerer Eingabe
//
// 07.04.2012: FABR

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 20

// Read a string from stdin into buffer
// Return true on success, false on error
bool ngets(char *buffer, int bufsize) {
    char *res = fgets(buffer,bufsize,stdin);
    if (res != NULL) {
        // fgets() guarantees at least one \0 in the buffer
        int len = strlen(buffer);
        if (len > 0) {
            if (buffer[len -1] != '\n') {
                // Attempt for buffer overrun; discard rest of input buffer
                while (fgetc(stdin) != '\n');
            }
            // In all cases delete character prior to \0
            // This is usually a \n if we got no buffer overrun
            buffer[len-1] = '\0';
        }
    }
    return (res != NULL);
}

int main() {
    pid_t pid;
    int status;
    char cmd[BUFSIZE];

    while(1) {
        // Prompt ausgeben
        printf("shell > ");
        fflush(stdout);

        // Kommando einlesen; Ende bei leerer Eingabe
        if(! ngets(cmd, BUFSIZE) ) {
            printf("Fehler beim Lesen von stdin\n");
            exit(EXIT_FAILURE);
        } else if (cmd[0] == '\0') {
            printf("Shell wird beendet\n");
            exit(EXIT_SUCCESS);
        }

        // Kommando ausfuehren
        pid = fork();
        if (pid < 0) {
            printf("Fehler bei fork()\n");
            exit(EXIT_FAILURE);
        } else if (pid==0) {
            // Kindprozess
            if(execl(cmd,cmd, (char *) NULL) < 0) {
                // Hier sollten wir nie vorbeikommen
                // execl kommt nur im Fehlerfall zurueck zum Aufrufer
                printf("Fehler in execl: kann Kommando \n\t%s\nnicht ausfuehren\n", cmd);
                exit(EXIT_FAILURE);
            }
        } else {
            // Elternprozess
            printf("Warten auf Kind %d\n", (int) pid);
            pid = wait(&status);
            if (WIFEXITED(status)) {
                printf("Rueckgabewert von Kind %d ist %d\n", (int) pid, WEXITSTATUS(status));
            } else {
                printf("Kindprozess %d abnormal abgebrochen\n", (int) pid);
            }
        }
    } // while(1)
    // Wegen Endlosschleife nicht erreichbar
}
