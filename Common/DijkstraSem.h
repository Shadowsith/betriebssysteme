/*
   A Dijkstra Semaphore Interface
   Implementations may be done by using POSIX or SYSV semaphores.
   Simplified and specialized to Linux
   01.06.2012  by FABR

   Derived from:
   Simple Semaphore Interface - POSIX implementation
   AUP2, Sec. 7.10.1

 */
#ifndef _DIJKSTRA_SEM_H_
#define _DIJKSTRA_SEM_H_

#include <stdbool.h>

struct DijkstraSem {
    union {
        int sm_semid;
        void *sm_sem;
    } sm;
};

// for convenience
typedef struct DijkstraSem sema;

/*******************************************************************
 * Dijkstra like P/V operations on SEMAPHORES
 *
 * For the etymology of P/V see http://en.wikipedia.org/wiki/Semaphore_(programming)
 *
 * P: probeer te verlagen (try to decrease)
 * V: verhogen (increase)
 *
 *******************************************************************
 */

// Post/increment
bool V(sema *sem);

// Wait/decrement
bool P(sema *sem);

/*******************************************************************
 * NAMED SEMAPHORES
 *******************************************************************
 */

// Open named semaphore and create with initial value if not existent
// Returns pointer to semaphore, NULL if failure
sema *DijkstraSemCreate(const char *name, unsigned int val);

// Try to open named semaphore, fail if semaphore does not yet exist
// Returns pointer to semaphore, NULL if failure
sema *DijkstraSemOpen(const char *name);

// Close named semaphore handle
bool DijkstraSemClose(sema *sem);

// Mark named semaphore to be removed. Will be removed by system when
// as soon as no references to it are left.
bool DijkstraSemRemove(const char *name);

/*******************************************************************
 * UNNAMED SEMAPHORES
 *******************************************************************
 */

// Create unnamed semaphore with initial value
// Returns pointer to semaphore, NULL if failure
sema *DijkstraSemCreateNN(unsigned int val);

// Close unnamed semaphore handle.
// Ressources of calling process are freed.
// There should not be left any references to the semaphore.
bool DijkstraSemCloseNN(sema *sem);

#endif /* _DIJKSTRA_SEM_H_ */
