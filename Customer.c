/***************************************************************
Gedd Johnson
Customer.c
Project 2:  A Queue Simulation

Provides the function to generate a new customer based on a given time.
***************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include"Customer.h"

custP newCustomer(double aTime){
	custP customer = (custP)malloc(sizeof(struct Customer));
	customer->arrivalTime = aTime;
	customer->PQtime = aTime;
	customer->departureTime = -1;
	customer->nextCust = NULL; // use only for FIFO

	return customer;
}