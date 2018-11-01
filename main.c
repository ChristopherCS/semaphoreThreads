// Semaphore Assignment
// BY: Chris Carlson
// Description: Creates 9 processes all condending for 2 semaphores. Each process counts
// the number of deadlocks it had to recover from, and prints this to screen when it exits.


//#define _POSIX_C_SOURCE 199309L // Needed to expose the CLOCK_REALTIME definition
#include <time.h> // For the timespec struct. Used in sem_timedwait.
#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h> 
#include <stdlib.h> 
#include <sys/wait.h> 
#include <unistd.h> 
#include <pthread.h>
#include "main.h"


int main(void){
	sem_t semS, semK;
	pthread_t threadIds[9];
	int nthreads = 9;
	thArgs args[9];

	int i = 0, ret = 0;
	
	//Init both semaphores
	initSemaphore(&semS);
	initSemaphore(&semK);

  // spawn 9 threads  
  for(i = 0; i < nthreads; i++){
		args[i].index = i+1;
		args[i].semK = &semK;
		args[i].semS = &semS;

		pthread_create(&threadIds[i], NULL, process, &args[i]);
	}

	for(i=0; i < nthreads; i++){
		pthread_join(threadIds[i], NULL);
	}

	closeSemaphores(&semS, &semK);
    
  
return ret;
   
//End Mother process


}


void *process(void *thdArgStruct){

thArgs *args = (thArgs *)thdArgStruct;
int index = args->index;
sem_t *semS = args->semS;
sem_t *semK = args->semK;
void *ret = NULL;
	
// sem_open both semaphores if necessary
	int deadlocks = 0;
	char *input = (char *)calloc(sizeof(char), 128); // For getting input from the command line]
	struct timespec sleepTime = {.tv_nsec = 100000, .tv_sec = 0};

// Sleep for a sec because I'm not getting deadlocks.
nanosleep(&sleepTime, NULL);

// 	until quit, while not q
	while(input[0] != 'q'){
	// Even
	if( index % 2 == 0){
		deadlocks += getSemaphores(semS, semK, index);
	// Else Odd Opposite Order
	}else{
		deadlocks += getSemaphores(semK, semS, index);
	}
	// Just sleep for a second because otherwise the 
	// incoming thread  the outgoing threads report. (sometimes).
	nanosleep(&sleepTime, NULL);
	//	   prompt "enter < 80 characters or q to quit: "
	printf("Enter < 80 characters or q to quit: ");

  //	   read keyboard
	if(fgets(input, 80, stdin) != NULL){
	  // echo what was typed
		printf("-> %s\n", input);
	}else{
		printf("\n");
		input[0] = ' ';
	}


  // 	   sem_post or give back both semaphores
	returnSemaphore(semS);
	returnSemaphore(semK);
	}
	// 	 prompt "This process had " + count + " deadlocks "
	printf("** Thread # %d, had %d deadlocks **\n", index, deadlocks);

	// exit
	free(input);
	return(ret); 	
}





// returns count of how many times recovered from deadlock
int getSemaphores(sem_t *semA, sem_t *semB, int index){
	int deadlocks = 0, waitResult;
	struct timespec waitTime, sleepTime;
	srand(time(NULL));

	// Get the first semaphore
	sem_wait(semA);

	// Get random wait time and the second semaphore
	getWaitTimeNano(&waitTime);
	waitResult = sem_timedwait(semB, &waitTime);
	while(waitResult != 0){
		//Increment Deadlock Counter
		deadlocks++;
		//Return the Semaphore we have
		returnSemaphore(semA);

		// Sleep
		getWaitTimeSeconds(&sleepTime);
		sleep(sleepTime.tv_sec);

		// Reaqquire the first semaphore
		sem_wait(semA);
		// Update the timeStruct
		// And try waiting again.	
		getWaitTimeNano(&waitTime);
		waitResult = sem_timedwait(semB, &waitTime);
	}
	return(deadlocks);
}



void getWaitTimeSeconds(struct timespec *tSpec){
	int seconds;
	
	// Fill the timeStruct
	//clock_gettime(CLOCK_REALTIME, tSpec);	

	seconds = rand() % 5 + 1;

	tSpec->tv_nsec = 0;
	tSpec->tv_sec = seconds;

}

void getWaitTimeNano(struct timespec *tSpec){
	long nanos;
	
	// Fill the timeStruct
	//clock_gettime(CLOCK_REALTIME, tSpec);

	nanos = rand() % 1000 * 10000;
	tSpec->tv_sec = 0;
	tSpec->tv_nsec = nanos;	
}

void initSemaphore(sem_t *semaphore){
	if( sem_init(semaphore, 0, 1) ){
		perror("Unable to initiate semaphore\n");
		exit(1);
	}
}

sem_t *openSemaphore(sem_t *semaphore, char *semaphoreName){
	
	semaphore = sem_open(semaphoreName, O_CREAT, 0644, 1);
	if(semaphore == SEM_FAILED){
		perror("Failed to open semaphore\n");
		exit(1);
	}
	return(semaphore);
}

void returnSemaphore(sem_t *sema){
	int error = 0;

	error += sem_post(sema);

	if(error){
		perror("Unable to return semaphore from process\n");
		exit(1);
	}

}

void closeSemaphores(sem_t *semS, sem_t *semK){
	int error = 0;

	error += sem_destroy(semS);
	error += sem_destroy(semK);
	if(error){
		perror("Unable to close semaphore\n");
		exit(1);
	}
}