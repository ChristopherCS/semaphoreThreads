// Semaphore Assignment
// BY: Chris Carlson
// Description: Creates 9 processes all condending for 2 semaphores. Each process counts
// the number of deadlocks it had to recover from, and prints this to screen when it exits.

#include "main.h"

int main(void){
	sem_t *semS, *semK;
	char *semSName = "/semS", semKName = "/semK";
	int pids[9]; int pid;
	int i = 0;
	int ret = 0;

	//sem_open both semaphores
	semS = openSemaphore(semS, semSName);
	semK = openSemaphore(semK, semKName);

  // fork 9 processes   
  do {
    if( (pid = fork()) == 0)
	process(semS, semK, i);
    else {
      pids[i] = pid;
      i++;
    }
  } while( i < 9 && pid > 0);
  if(i >= 9) { // stop forking around
     // after all die, use single call or loop based on ids saved above
		waitpid(-1, NULL, WEXITED);
    sem_unlink(semS);
		sem_unlink(semK);
  }
return 0;
   
//End Mother process


}


void process(sem_t * semS, sem_t *semK, int index){

// sem_open both semaphores if necessary
	int deadlocks = 0;
	char *input = (char *)calloc(sizeof(char), 128); // For getting input from the command line
	
// 	until quit while not q
	while(input[0] != 'q'){
	// 	count += getSemaphores(index)
	deadlocks += getSemaphores(semS, semK, index);
	//	   prompt "enter < 80 characters or q to quit: "
	printf("This is process #%d\n", index);
	printf("Enter < 80 characters or q to quit: ");

  //	   read keyboard
	scanf("%80s",input );

//   	   echo what was typed
	printf("%s", input);
// 	   sem_post or give back both semaphores
	returnSemaphores(semS, semK);
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

void returnSemaphores(sem_t *semS, sem_t *semK){
	int error = 0;

	error += sem_post(semS);
	error += sem_post(semK);

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
	int deadlocks = 0;
	struct timespec waitTime;
	
	clock_gettime(CLOCK_REALTIME, waitTime):
	
//   odd index gets screen first
	if(index%2 == 1){
		sem_wait(semS);
		

	}else{
//   even index gets keyboard first
		sem_wait(semK);
	}
//   count = 0
//   loop
//   	sem_wait for first
// 	sleep(1) // only use this if no deadlocks without it should not be necessary
//   	sem_timedwait with timer for second
//    	if timeout 
//      		give back first, (sem_post)
//      		wait random time
// 		increment count
//    	else
//      		have both  true   
//  until have both

	return(deadlocks);
}