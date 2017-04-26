// EXEC-Demo
// Das letze Programm im Prozess

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
    printf("PID=%d: Ich bin das Ende der Kette\n", (int) getpid() );
    sleep(3);

    exit(EXIT_SUCCESS);
}
