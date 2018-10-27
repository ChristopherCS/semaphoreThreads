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
	char *semSName = "/semS", *semKName = "/semK";
	int pid, pids[9];
	pid_t waitPid;
	int i = 0, status, ret = 0;
	

	//sem_open both semaphores
	printf("Opening Semaphores\n");
	semS = openSemaphore(semS, semSName);
	semK = openSemaphore(semK, semKName);
  // fork 9 processes   
  do {
    if( (pid = fork()) == 0){
			process(semS, semK, i);
		}else {
			pids[i++] = pid;
    }
  } while( i < 9 && pid > 0);
  if(i >= 9) { // stop forking around
     // after all die, use single call or loop based on ids saved above
		 i--;
		while((waitPid = wait(&status))> 0){
			printf("%d children left, PID: %d\n",i, waitPid);
			i--;
		}
		printf("All Children Returned. Unlinking Semaphores.\n");
    sem_unlink(semSName);
		sem_unlink(semKName);
  }
return ret;
   
//End Mother process


}


void process(sem_t * semS, sem_t *semK, int index){

	
// sem_open both semaphores if necessary
	int deadlocks = 0;
	char *input = (char *)calloc(sizeof(char), 128); // For getting input from the command line
	
	printf("Process Child # %d\n", index);
	input[0] = 's';
	printf("input[0] = %c\n", input[0]);
// 	until quit while not q
	while(input[0] != 'q'){
	deadlocks += getSemaphores(semS, semK, index);
	//	   prompt "enter < 80 characters or q to quit: "
	printf("This is process #%d\n", index);
	printf("Enter < 80 characters or q to quit: ");

  //	   read keyboard
	scanf("%80s",input );

//   	   echo what was typed
	printf("%s", input);
// 	   sem_post or give back both semaphores
	returnSemaphore(semS);
	returnSemaphore(semK);
	}
// 	 prompt "This process had " + count + " deadlocks "
printf("Process # %d, had %d deadlocks", index, deadlocks);

closeSemaphores(semS, semK);


// exit

	free(input);
}



sem_t *openSemaphore(sem_t *semaphore, char *semaphoreName){
	
	semaphore = sem_open("/semS", O_CREAT, 0644, 1);
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

// returns count of how many times recovered from deadlock
int getSemaphores(sem_t *semS, sem_t *semK, int index){
	int deadlocks = 0, waitResult;
	struct timespec waitTime, sleepTime;
	srand(time(NULL));
	

	// Fill the timeStruct
	clock_gettime(CLOCK_REALTIME, &waitTime);	
	
	// Add some time to the nanoseconds file.
	waitTime.tv_nsec = waitTime.tv_nsec + 9000000; // Add 1 Milisecond

	printf("Process %d about to wait for semaphores.\n", index);
//   odd index gets screen first
	if(index%2 == 1){
		sem_wait(semS);
		printf("Process %d has semaphore for Screen\n", index);
		
		waitResult = sem_timedwait(semK, &waitTime);
		while(waitResult != 0){
			printf("Process %d giving back semaphore for Screen.\n", index);
			//Increment Deadlock Counter
			deadlocks++;
			//Return the Semaphore we have
			returnSemaphore(semS);

			//Refill and reset the sleep struct
			clock_gettime(CLOCK_REALTIME, &sleepTime);
			sleepTime.tv_nsec += (rand()%1000) * 1000;
			printf("Process %d going to sleep for %ld\n", index, sleepTime.tv_nsec);
			// Sleep
			nanosleep(&sleepTime, NULL);
	
		// Reaqquire the first semaphore
		sem_wait(semS);
		// Update the timeStruct
		clock_gettime(CLOCK_REALTIME, &waitTime);	
		waitTime.tv_nsec = waitTime.tv_nsec + 9000000; // Add 9 Miliseconds
	// And try waiting again.
    waitResult = sem_timedwait(semK, &waitTime);
		}

	}else{
//   even index gets keyboard first
		sem_wait(semK);
		waitResult = sem_timedwait(semS, &waitTime);
		while(waitResult != 0){
			//Increment Deadlock Counter
			deadlocks++;
			//Return the Semaphore we have
			returnSemaphore(semK);

			//Refill and reset the sleep struct
			clock_gettime(CLOCK_REALTIME, &sleepTime);
			sleepTime.tv_nsec += (rand()%100) * 1000;
			// Sleep
			nanosleep(&sleepTime, NULL);
	
		// Reaqquire the first semaphore
		sem_wait(semK);
		// Update the timeStruct
		clock_gettime(CLOCK_REALTIME, &waitTime);	
		waitTime.tv_nsec = waitTime.tv_nsec + 9000000; // Add 9 Miliseconds
	// And try waiting again.
		waitResult = sem_timedwait(semS, &waitTime);
		}
	}
	return(deadlocks);
}