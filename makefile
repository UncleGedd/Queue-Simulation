CFLAGS = -g -Wall
CC = gcc

objects = Queue.o PQ.o Customer.o main.o

main: $(objects)
	gcc -g -Wall -o main $(objects) -lm

PQ.o:		PQ.c PQ.h
Customer.o:	Customer.c Customer.h
Queue.o: 	Queue.c Queue.h
main.o: 	main.c PQ.h Customer.h Queue.h

.PHONY : clean
clean: 
	rm main $(objects)
