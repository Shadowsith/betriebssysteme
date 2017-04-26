// Code derived from
// http://jimscode.ca/index.php/component/content/article/13-c/122-c-clockgettime-example
//
// 2012-04-13  fabr
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "nsectimer.h"

#define SEC2NANO 1000000000
#define MAXTIMERNAME 256

struct nsectimer {
    char timername[MAXTIMERNAME];
    struct timespec starttime;
    struct timespec endtime;
    long long totaltime;
};

struct nsectimer* createTimer(char * timername) {
    struct nsectimer* retval;

    retval = (struct nsectimer*) malloc(sizeof(struct nsectimer));
    if (retval == NULL) {
        perror("createTimer: out of memory");
        exit(EXIT_FAILURE);
    }
    memset(retval, 0, sizeof(struct nsectimer));
    strncpy(retval->timername, timername, MAXTIMERNAME);
    return retval;
}

int startTimer(struct nsectimer* obj) {
    int retval = 0;

    retval = clock_gettime(CLOCK_REALTIME, &(obj->starttime));
    if (-1 == retval) {
        perror("startTimer : clock_getres");
    }
    return retval;
}

int endTimer(struct nsectimer* obj) {
    int retval = 0;
    retval = clock_gettime(CLOCK_REALTIME, &(obj->endtime));
    if (-1 == retval) {
        perror("endTimer : clock_gettime");
        return retval;
    } else {
        obj->totaltime +=
            (((long long) obj->endtime.tv_sec * SEC2NANO) + obj->endtime.tv_nsec) -
            (((long long) obj->starttime.tv_sec * SEC2NANO) + obj->starttime.tv_nsec);
        return retval;
    }
}

void displayTimer(struct nsectimer* obj) {
    printf("%s: %lld ns (%g s)\n",
            obj->timername,
            obj->totaltime,
            obj->totaltime / ((double) SEC2NANO));
}

double getTimer(struct nsectimer* obj) {
    return obj->totaltime / ((double) SEC2NANO);
}

void destroyTimer(struct nsectimer* obj) {
    free(obj);
} 

