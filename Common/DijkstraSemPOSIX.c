/*
   A POSIX implementation of a Dijkstra Semaphore Interface
   Simplified and specialized to Linux
   01.06.2012  by FABR

   Derived from:
   Simple Semaphore Interface - POSIX implementation
   AUP2, Sec. 7.10.1

 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>

#include "DijkstraSem.h"

#define PERM_FILE                (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

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

bool V(sema *sem) {
    if( sem_post(sem->sm.sm_sem) == -1 ) {
        return false;
    }
    return true;
}

bool P(sema *sem) {
    if( sem_wait(sem->sm.sm_sem)  == -1 ) {
        return false;
    }
    return true;
}

/*******************************************************************
 * NAMED SEMAPHORES
 *******************************************************************
 */

// Open named semaphore and create with initial value if not existent
// Returns pointer to semaphore, NULL if failure
sema *DijkstraSemCreate(const char *name, unsigned int val) {
    sema *sem = NULL;

    if ( (sem = malloc(sizeof(sema))) == NULL ) {
        return NULL;
    }

    if ((sem->sm.sm_sem = sem_open(name, O_CREAT, PERM_FILE, val)) == SEM_FAILED) {
        free(sem);
        return NULL;
    }
    return sem;

}

// Try to open named semaphore, fail if semaphore does not yet exist
// Returns pointer to semaphore, NULL if failure
sema *DijkstraSemOpen(const char *name) {
    sema *sem = NULL;

    if ( (sem = malloc(sizeof(sema))) == NULL ) {
        return NULL;
    }

    if ((sem->sm.sm_sem = sem_open(name, 0)) == SEM_FAILED) {
        free(sem);
        return NULL;
    }
    return sem;

}

// Close named semaphore handle
bool DijkstraSemClose(sema *sem) {
    if( sem_close(sem->sm.sm_sem) == -1 ) {
        return false;
    }
    free(sem);
    return true;
}

// Mark named semaphore to be removed. Will be removed by system when
// as soon as no references to it are left.
bool DijkstraSemRemove(const char *name) {
    if (sem_unlink(name) == -1 && errno != ENOENT) {
        return false;
    }
    return true;
}

/*******************************************************************
 * UNNAMED SEMAPHORES
 *******************************************************************
 */

// Create unnamed semaphore with initial value
// Returns pointer to semaphore, NULL if failure
sema *DijkstraSemCreateNN(unsigned int val) {
    sema *sem = NULL;

    // Allocate memory for struct DijkstraSem == sema
    if ( (sem = malloc(sizeof(sema))) == NULL ) {
        return NULL;
    }

    // Allocate memory for sem_t object
    if ( (sem->sm.sm_sem = malloc(sizeof(sem_t)) ) == NULL ) {
        free(sem);
        return NULL;
    }

    // Initialize sem_t object
    if ( -1 == sem_init(sem->sm.sm_sem, 0 , val) ) {
        free (sem->sm.sm_sem);
        free (sem);
        return NULL;
    }

    return sem;
}

// Close unnamed semaphore handle.
// Ressources of calling process are freed.
// There should not be left any references to the semaphore.
bool DijkstraSemCloseNN(sema *sem) {
    if (-1 == sem_destroy(sem->sm.sm_sem) ) {
        return false;
    }

    free(sem->sm.sm_sem);
    free(sem);
    return true;
}

