// EXEC-Demo
// Das mittlere Programm im Prozess

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
    printf("PID=%d: Ich bin das mittlere Programm der Kette\n", (int) getpid() );
    sleep(3);

    if( execl("./bin/tailprog","./bin/tailprog",NULL) ) {
        // Sollte nicht vorkommen
        perror("Kann Verkettung von ./bin/tailprog nicht ausfuehren");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
