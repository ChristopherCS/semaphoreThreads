CC = gcc
CFLAGS = -g -Wall -pedantic
LIBRARIES = -lpthread -lrt 

main : main.o  
	${CC} ${CFLAGS} ${LIBRARIES} -o semaphore main.o  
	
main.o : main.h


.PHONY : clean remake


clean: 
	rm -f *.o core semaphore *.log

remake :
	make clean
	make