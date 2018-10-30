// Semaphore Assignment
// BY: Chris Carlson
// Description: header file for main.c of the semaphores project. 

#ifndef MAIN_HEADER_FILE
#define MAIN_HEADER_FILE

typedef struct {
	sem_t *semS;
	sem_t *semK;
	int index;
} thArgs;

void *process(void *thdArgStruct);
sem_t *openSemaphore(sem_t *semaphore, char *semaphoreName);
void initSemaphore(sem_t *semaphore);
int getSemaphores(sem_t *semS, sem_t *semK, int index);
void returnSemaphore(sem_t *sema);
void closeSemaphores(sem_t *semS, sem_t *semK);
void getWaitTimeSeconds(struct timespec *tSpec);
void getWaitTimeNano(struct timespec *tSpec);
#endif