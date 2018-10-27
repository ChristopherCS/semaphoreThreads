CC = gcc
CFLAGS = -g -Wall -pedantic
LIBRARIES = -lpthread -lrt 

main : main.o  
	${CC} ${CFLAGS} -o semaphore main.o  ${LIBRARIES}
	
main.o : main.h


.PHONY : clean remake


clean: 
	rm -f *.o core semaphore *.log

remake :
	make clean
	make