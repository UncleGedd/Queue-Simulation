/***************************************************************
Gedd Johnson
Queue.c
Project 2:  A Queue Simulation

Private implementation of the FIFO queue
***************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include"Queue.h"
#include"Customer.h"


struct Queue{
	custP head;
	custP tail;
	int numFIFO;
};


queueP createQueue(){
	queueP queue = (queueP)malloc(sizeof(struct Queue));

	if (queue == NULL){
		fprintf(stderr, "Memory allocation has failed\n");
		return NULL;
	}

	queue->head = NULL;
	queue->tail = NULL;
	queue->numFIFO = 0;
	return queue;
}


void enQueue(queueP queuePtr, custP customer){

	// check if queue is empty
	if (queuePtr->head == NULL){
		queuePtr->head = customer;
		queuePtr->tail = customer;
		queuePtr->numFIFO++;
		return;
	}

	// link last node of queue to new customer and set tail to new customer
	queuePtr->tail->nextCust = customer;
	queuePtr->tail = customer;
	queuePtr->numFIFO++;
	return;
}


custP deQueue(queueP queuePtr){
	if (queuePtr->head == NULL){ // queue is empty
		return NULL;
	}
	custP tmp = queuePtr->head; // use for returning
	queuePtr->head = queuePtr->head->nextCust; // move head to the next Customer
	return tmp; // return the dequeued Customer
}


void printQueue(queueP queuePtr){
	if (queuePtr == NULL || queuePtr->head == NULL){
		fprintf(stderr, "No queue available or empty queue\n");
		return;
	}

	custP q = queuePtr->head;
	
	while (q != NULL){
		printf("PQ time = %f\n", q->PQtime);
		q = q->nextCust;
	}
	
	printf("Total customers who had to wait in FIFO = %d\n", queuePtr->numFIFO);
	return;
}

queueP freeQueue(queueP qPtr){
	custP q = qPtr->head;
	custP temp;

	// Ideally this will never execute because all customers in the FIFO will go into the PQ
	while (q != NULL){
		temp = q;
		q = q->nextCust;
		free(temp);
	}
	free(qPtr);
	qPtr = NULL;
	return qPtr;
}