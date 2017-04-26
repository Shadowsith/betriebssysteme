#ifndef NSECTIMER_H_
#define NSECTIMER_H_

// NOTE:
// Users of this library function will only use
// pointers to struct nsectimer.
// Therefore, the definition of the structure does not need
// to be revealed in the interface.

struct nsectimer* createTimer();
int startTimer(struct nsectimer* obj);
int endTimer(struct nsectimer* obj);
void displayTimer(struct nsectimer* obj); 
double getTimer(struct nsectimer* obj);
void destroyTimer(struct nsectimer* obj);

#endif /* NSECTIMER_H_ */
