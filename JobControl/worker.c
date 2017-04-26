// Worker process for demo of job control
// 08.04.12  fabr

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// calibration
#define FACTOR 100

void do_work() { // let the cpu do some work
    long i;
    float x=0.0;

    for (i=0;i< FACTOR*800000;i++) {
        x+=92.77/29388;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("usage: worker rounds\n");
        exit(1);
    }

    int rounds = atoi(argv[1]);
    int count = 0;
    while(count < rounds) {
        do_work();
        count++;
        printf("PID=%d: %d\n",getpid(),count);
    }
    return 0;
}
