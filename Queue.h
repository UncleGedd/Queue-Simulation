/***************************************************************
Gedd Johnson
PQ.h
Project 2:  A Queue Simulation

Public implementation of the FIFO queue ADT
***************************************************************/
#include"Customer.h"

#ifndef _Queue_h
#define _Queue_h

typedef struct Queue *queueP;

/**
* Creates a new FIFO queue data structure
*
* return:  pointer to the queue
*/
queueP createQueue();

/**
* Places a new customer at the end of the queue
*
* @params:  pointer to the queue, pointer to the customer
*/
void enQueue(queueP, custP);

/**
* Retrieves the customer at the head of the queue
*
* @params: pointer to the queue
* return: pointer to the dequeued customer
*/
custP deQueue(queueP);


/**
*
* Prints the contents of the queue
*
* @params: pointer to the queue
*/
void printQueue(queueP);

/**
* Frees the memory used by the queue
*
* @params: queue pointer
* return:  a NULL queue pointer
*/
queueP freeQueue(queueP);

#endif