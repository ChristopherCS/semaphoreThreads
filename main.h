// Semaphore Assignment
// BY: Chris Carlson
// Description: header file for main.c of the semaphores project. 

#ifndef MAIN_HEADER_FILE
#define MAIN_HEADER_FILE

void process(sem_t * semS, sem_t *semK, int index);
sem_t *openSemaphore(sem_t *semaphore, char *semaphoreName);
int getSemaphores(sem_t *semS, sem_t *semK, int index);
void returnSemaphore(sem_t *sema);
void closeSemaphores(sem_t *semS, sem_t *semK);
void getWaitTimeSeconds(struct timespec *tSpec);
void getWaitTimeNano(struct timespec *tSpec);
#endif