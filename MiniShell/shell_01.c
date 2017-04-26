// shell_01.c
// Minimale shell ohne Fehlerbehandlung
// Verbesserung mit Fehlerbehandlung in shell_02.c
//
// Abbruch der Endlosschleife mit Ctrl-C
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
    char cmd[BUFSIZE];
    while(1) {
        // Prompt ausgeben
        printf("shell > ");
        fflush(stdout);

        // Kommando einlesen; Keine Fehlerbehandlung vorerst!
        ngets(cmd, BUFSIZE);

        // Kommando ausfuehren
        pid = fork();
        if (pid==0) {
            // Kindprozess
            execl(cmd,cmd, (char *) NULL);
            exit(EXIT_FAILURE);
        } else {
            // Elternprozess
            wait(NULL);
        }
    } // while(1)
}
