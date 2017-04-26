// Worker process for demo of nice utility
// 28.03.12  fabr

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
		printf("usage: worker tabs\n");
		exit(1);
	}

	int tabs = atoi(argv[1]);
	int count = 0;
	while(1) {
		do_work();
		count++;

		int j;
		for(j=0; j< tabs; j++) printf("\t");
		printf("PID=%d: %d\n",getpid(),count);
	}
	return 0;
}
