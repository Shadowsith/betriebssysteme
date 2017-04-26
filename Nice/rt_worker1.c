// Worker process for demo of realtime scheduling policies
// 29.05.2012  fabr

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>

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
		printf("usage: rt_worker1 tabs\n");
		exit(1);
	}

	int tabs = atoi(argv[1]);
	struct sched_param sp;
	int prio;

	int count = 0;
	//while(1) {
	while(count < 200) {
		do_work();
		count++;

		if( sched_getparam(0,&sp) == -1){
			perror("Unable to get realtime priority of process");
			exit(EXIT_FAILURE);
		}

		if( (prio=getpriority(PRIO_PROCESS,0)) == -1){
			perror("Unable to get nice value of process");
			exit(EXIT_FAILURE);
		}

		int j;
		for(j=0; j< tabs; j++) printf("\t");

		if(sp.sched_priority > 0) {
			printf("PID=%d REALTIME_PRIO=%d PRIO= %d count=%d\n",
					getpid(),sp.sched_priority,
					-sp.sched_priority-1,count);
		} else {
			printf("PID=%d PRIO= %d count=%d\n",
					getpid(),prio+20,count);
		}

	}
	return 0;
}
