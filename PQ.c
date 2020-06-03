/***************************************************************
Gedd Johnson
PQ.c
Project 2:  A Queue Simulation

Private implementation of the PQ ADT
***************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include"PQ.h"
#include"Customer.h"

/*
* Use percolateUp() for inserting items into the PQ
*
* @params: PQ pointer, customer to add to the PQ
*/
void percolateUp(pqP, custP);

/*
* Use percolateDown() for removing items from the PQ
*
* @params: PQ pointer, slot=1 for removing (slot is an index, therefore an int)
*/
void percolateDown(pqP, int slot);

pqP createPQ(){
	pqP pqPtr = (pqP)malloc(sizeof(struct PQ));

	// intialize all of the custP's in the PQ array to NULL and init counters and running sums
	int i;
	for (i = 1; i < PQSIZE+1; i++)
		pqPtr->customerPQ[i] = NULL;
	pqPtr->theSize = 0;
	pqPtr->totalServiceTime = 0;
	pqPtr->Po_sim = 0;
	pqPtr->numCustomerWaiting_sim = 0;
	pqPtr->totalWaitTime = 0;
	return pqPtr;
}

void InitEnPQ(pqP pqPtr, custP customer){
	if (pqPtr->theSize >= PQSIZE){ // if PQ is at capacity
		printf("PQ is currently at capacity\n");
		return;
	}
	// check if PQ is empty
	else if (pqPtr->theSize == 0){
		pqPtr->customerPQ[1] = customer;
		pqPtr->theSize++; // theSize = 1
		return;
	}

	else { // place next customer in PQ
		// after first customer had been added to PQ at [1], the second will be added at [2] (numCustomers + 1)
		pqPtr->customerPQ[pqPtr->theSize + 1] = customer;
		pqPtr->theSize++;
		return;
	}
}

void enPQ(pqP pqPtr, custP customer){
	if (pqPtr->theSize == PQSIZE){
		fprintf(stderr, "PQ is at max capacity\n");
		return;
	}

	pqPtr->theSize++; // increment theSize
	percolateUp(pqPtr, customer);
	return;
}


custP dePQ(pqP pqPtr){
	if (pqPtr->theSize == 0){
		fprintf(stderr, "No customers in PQ\n");
		return NULL;
	}

	custP served = pqPtr->customerPQ[1]; // copy customer to be served to a temp variable
	pqPtr->customerPQ[1] = pqPtr->customerPQ[pqPtr->theSize]; // copy [theSize] to [1]
	pqPtr->theSize--; // decrement theSize, logically removing the last element of the array
	percolateDown(pqPtr, 1); // percolate down to keep the PQ heap true
	return served; // return the customer to be served
}

void printPQ(pqP pqPtr){
	int i;
	printf("\nNumber of customers in PQ = %d\n", pqPtr->theSize);

	for (i = 1; i <= pqPtr->theSize; i++){
		if (pqPtr->customerPQ[i] == NULL)
			return;

		else if (pqPtr->customerPQ[i]->departureTime == -1)
			printf("[%d] %lf Arrival\n", i, pqPtr->customerPQ[i]->PQtime);

		else printf("[%d] %lf Departure\n", i, pqPtr->customerPQ[i]->PQtime);
	}
	printf("\n");
	return;
}

pqP freePQ(pqP pqPtr){
	// no need to free customers as they are freed when they are dePD'd by departures
	free(pqPtr); // free PQ data stucture
	pqPtr = NULL;
	return pqPtr;
}

void percolateUp(pqP pqPtr, custP customer){
	pqPtr->customerPQ[0] = customer; // init sentinel
	int slot = pqPtr->theSize;

	while (customer->PQtime < pqPtr->customerPQ[slot / 2]->PQtime){
		pqPtr->customerPQ[slot] = pqPtr->customerPQ[slot / 2];
		slot /= 2;
	}
	pqPtr->customerPQ[slot] = customer;
	return;
}


void percolateDown(pqP pqPtr, int slot){
	int child;
	custP temp = pqPtr->customerPQ[slot];

	while (slot * 2 <= pqPtr->theSize){
		child = slot * 2; // index of left child

		// if this is true, a right child exists AND it is higher priority than the left child
		if (child != pqPtr->theSize && pqPtr->customerPQ[child + 1]->PQtime < pqPtr->customerPQ[child]->PQtime)
			child++; // index of right child

		if (pqPtr->customerPQ[child]->PQtime < temp->PQtime) // swap
			pqPtr->customerPQ[slot] = pqPtr->customerPQ[child];

		else break; // slot now points to the location where temp should be placed

		slot = child;
	}
	pqPtr->customerPQ[slot] = temp;
	return;
}