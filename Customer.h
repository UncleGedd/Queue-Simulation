/***************************************************************
Gedd Johnson
Customer.h
Project 2:  A Queue Simulation

Public implementation of the Customer ADT.  Note that
the Customer struct is made public because all files will use it.
***************************************************************/
#ifndef _Customer_h
#define _Customer_h

typedef struct Customer *custP;

struct Customer{
	double PQtime;
	double arrivalTime;
	double startOfServiceTime;
	double departureTime;
	struct Customer *nextCust; // use for FIFO queue list
};

/**
* Generates a new customer
* 
* @params: time the customer is placed in the PQ, could be arrival or departure
* returns: pointer to the new customer
*/
custP newCustomer(double);

#endif