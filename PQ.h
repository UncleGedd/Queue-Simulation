/***************************************************************
Gedd Johnson
PQ.h
Project 2:  A Queue Simulation

Public implementation of the PQ ADT
***************************************************************/
#include"Customer.h"

#ifndef _PQ_h
#define _PQ_h

#define PQSIZE 100

typedef struct PQ *pqP;

struct PQ{
	custP customerPQ[PQSIZE + 1];
	int theSize;
	int serverAvailableCount;
	double Po_sim; // the following are the simulation stats to be computed
	double L_sim;
	double W_sim;
	double Lq_sim;
	double Wq_sim;
	double rho_sim;
	double numCustomerWaiting_sim;
	double totalServiceTime;
	double totalWaitTime;
};

/*
* Create a new priority queue (PQ) data structure
*
* return: pointer to the PQ
*/
pqP createPQ();

/*
* Places the initial batch of customers in the PQ
*
* @params: PQ pointer, customer pointer
*/
void InitEnPQ(pqP, custP);

/*
* Place a customer in the PQ; heap conditions must hold!
*
* @params: PQ pointer, customer pointer
*/
void enPQ(pqP, custP);

/*
* Remove a customer from the head of the PQ
*
* @params: PQ pointer
* return: pointer to the customer that was dePQ'd
*/
custP dePQ(pqP);

/*
* Print the contents of the PQ
*
* @params: PQ pointer
*/
void printPQ(pqP pqPtr);

/*
* Free the PQ data structure
*
* @params: PQ pointer
* return: NULL PQ pointer
*/
pqP freePQ(pqP);

#endif