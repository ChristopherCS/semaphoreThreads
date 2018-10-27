// Semaphore Assignment
// BY: Chris Carlson
// Description: header file for main.c of the semaphores project. 

#ifndef MAIN_HEADER_FILE
#define MAIN_HEADER_FILE
#include <time.h> // For the timespec struct. Used in sem_timedwait.
#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h> 
#include <stdlib.h> 
#include <sys/wait.h> 
#include <unistd.h> 


void process(sem_t * semS, sem_t *semK, int index);
sem_t *openSemaphore(sem_t *semaphore, char *semaphoreName);
int getSemaphores(sem_t *semS, sem_t *semK, int index);
void returnSemaphores(sem_t *semS, sem_t *semK);
void closeSemaphores(sem_t *semS, sem_t *semK);
#endif