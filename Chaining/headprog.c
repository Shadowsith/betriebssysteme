// EXEC-Demo
// Das erste Programm im Prozess

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
    printf("PID=%d: Ich bin das erste Programm der Kette\n", (int) getpid() );
    sleep(3);

    if( execl("./bin/midprog","./bin/midprog",NULL) ) {
        // Sollte nicht vorkommen
        perror("Kann Verkettung von ./bin/midprog nicht ausfuehren");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
