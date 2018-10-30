// Semaphore Assignment
// BY: Chris Carlson
// Description: Creates 9 processes all condending for 2 semaphores. Each process counts
// the number of deadlocks it had to recover from, and prints this to screen when it exits.


#define _POSIX_C_SOURCE 199309L // Needed to expose the CLOCK_REALTIME definition
#include <time.h> // For the timespec struct. Used in sem_timedwait.
#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h> 
#include <stdlib.h> 
#include <sys/wait.h> 
#include <unistd.h> 
#include "main.h"


int main(void){
	sem_t *semS = NULL, *semK = NULL;
	char *semSName = "/semScreen", *semKName = "/semKeyboard";
	int pid;
	pid_t waitPid;
	int i = 0, status, ret = 0;
	

	//sem_open both semaphores
	semS = openSemaphore(semS, semSName);
	semK = openSemaphore(semK, semKName);
	
  // fork 9 processes   
  do {
    if( (pid = fork()) == 0){
			process(semS, semK, i);
		}else {
			i++;
    }
  } while( i < 9 && pid > 0);
  if(i >= 9) { // stop forking around
     // after all die, use single call or loop based on ids saved above
		while((waitPid = wait(&status))> 0);
    sem_unlink(semSName);
		sem_unlink(semKName);
  }
return ret;
   
//End Mother process


}


void process(sem_t * semS, sem_t *semK, int index){

	
// sem_open both semaphores if necessary
	int deadlocks = 0;
	char *input = (char *)calloc(sizeof(char), 128); // For getting input from the command line]

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
	// incoming thread overwrites the outgoing threads report. (sometimes).
	sleep(1);
	//	   prompt "enter < 80 characters or q to quit: "
	printf("Enter < 80 characters or q to quit: ");

  //	   read keyboard
	if(fgets(input, 80, stdin) != NULL){
	  // echo what was typed
		printf("-> %s\n\n", input);
	}else{
		printf("\n");
		input[0] = ' ';
	}


  // 	   sem_post or give back both semaphores
	returnSemaphore(semS);
	returnSemaphore(semK);
	}
	// 	 prompt "This process had " + count + " deadlocks "
	printf("\n** Process # %d, had %d deadlocks **\n\n", index, deadlocks);

	// exit
	closeSemaphores(semS, semK);
	free(input);
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


sem_t *openSemaphore(sem_t *semaphore, char *semaphoreName){
	
	semaphore = sem_open(semaphoreName, O_CREAT, 0644, 1);
	if(semaphore == SEM_FAILED){
		perror("Failed to open semaphore");
		exit(1);
	}
	return(semaphore);
}

void returnSemaphore(sem_t *sema){
	int error = 0;

	error += sem_post(sema);

	if(error){
		perror("Unable to return semaphore from process");
		exit(1);
	}

}

void closeSemaphores(sem_t *semS, sem_t *semK){
	int error = 0;

	error += sem_close(semS);
	error += sem_close(semK);
	if(error){
		perror("Unable to close semaphore");
		exit(1);
	}
}