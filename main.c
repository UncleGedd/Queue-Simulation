/***************************************************************
Gedd Johnson
main.c
Project 2:  A Queue Simulation

This file is the main implementation of the Queue Simulation.
***************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"Queue.h"
#include"PQ.h"
#include"Customer.h"

/**
* The following are math functions implementing the formulas from the project description
* 
* @params: lambda, mu, M, n; all read in from the file
* returns:  double (or long in the case of factorial())
*/
unsigned long factorial(double n); // computes factorial
double PoEq(double lambda, double mu, double M); // percent idle time
double LEq(double lambda, double mu, double M, double Po); // avg. number of customers in system
double WEq(double L, double lambda); // avg. time a customers spends in the system
double LqEq(double L, double lambda, double mu); // avg. number of customers in queue
double WqEq(double Lq, double lambda); // avg. time customers spends waiting in queue
double rhoEq(double lambda, double M, double mu); // utilization factor of system

/**
* Function that gets a random interval as described in the project description.
* Generates a random float between 0 and 1 and computes the interval with a 
* negative exponential distribution.
*
* @params: can be either mu or lambda depending on the interval desired
* return: double that is the next interval
*/
double getNextRandInterval(double);

/**
* Processes the next event in the PQ as either an arrival or departure and takes the appropriate actions including managing the fifo queue.
*
* @params: PQ pointer, FIFO queue pointer, customer pointer, mu, number of servers M
*/
void processNextEvent(pqP, queueP, custP, int, int);

/**
* Computes the ideal and simulated stats and prints them to the console
*
* @params: lambda, mu, M, totalCustomers, currentPQTime, pointer to PQ
*/
void computeStatsPrint(int, int, int, int, double, pqP);


int main(void){
	srand((unsigned int)time(NULL)); // set random seed, use unsigned int cast to silence the compiler
	int lambda, mu, M, n;
	int i;

	// get input from file
	FILE *fp = fopen("runSimulation.txt", "r");
	if (fp == NULL){
		fprintf(stderr, "Input file not opened, terminating program\n");
		return -1;
	}

	// read data from file into appropriate variables and close file
	fscanf(fp, "%d", &lambda);
	fscanf(fp, "%d", &mu);
	fscanf(fp, "%d", &M);
	fscanf(fp, "%d", &n); // number of customers to simulate
	fclose(fp);

	int totalCustomers = n; // hold this value because we decrement n throughout this simulation

	// create PQ and fifo queue
	pqP PQ = createPQ();
	queueP fifoQ = createQueue();
	double currentPQTime = 0;
	custP customer;

	// generate first customers and place into PQ
	for (i = 0; i < PQSIZE; i++){ 
		customer = newCustomer(currentPQTime);
		n = n - 1;

		if (n == 0) 
			break;
		currentPQTime += getNextRandInterval(lambda); // generate a random interval for next arrival

		// place in PQ
		InitEnPQ(PQ, customer);
	}
	
	// set server available count to M
	PQ->serverAvailableCount = M;

	// process first M arrivals creating M departure events, "prime the pump"
	for (i = 1; i <= M; i++){
		PQ->customerPQ[i]->startOfServiceTime = PQ->customerPQ[i]->arrivalTime;
		PQ->customerPQ[i]->departureTime = PQ->customerPQ[i]->startOfServiceTime + getNextRandInterval(mu); // generate service interval
		PQ->customerPQ[i]->PQtime = PQ->customerPQ[i]->departureTime;
		PQ->serverAvailableCount--;
	}

	while (PQ->theSize != 0){ // while PQ is not empty
		customer = dePQ(PQ); // serve customer off of PQ
		processNextEvent(PQ, fifoQ, customer, mu, M); // process next event

		if (PQ->theSize==1 && n == 0){ // last customer, get final PQ time for statistics
			currentPQTime = PQ->customerPQ[1]->PQtime;
		}
		
		if (n > 0 && PQ->theSize <= M + 1){ // add more customers to the PQ
			currentPQTime = PQ->customerPQ[M+1]->PQtime; // update current PQ time for new arrivals

			// generate next batch of arrivals
			for (i = 0; i < PQSIZE - (M+1); i++){
				if (n == 0) break; // no more customers

				// generate a customer
				customer = newCustomer(currentPQTime);
				n = n - 1;
				currentPQTime += getNextRandInterval(lambda); // generate a random interval for next arrival

				// place in PQ
				enPQ(PQ, customer);
			}
		}
	}

	// compute stats and print
	computeStatsPrint(lambda, mu, M, totalCustomers, currentPQTime, PQ);
	
	// free PQ and fifo queue
	freePQ(PQ);
	free(fifoQ);

	return 0;
}

double getNextRandInterval(double avg){
	double f = (double)rand() / (double)(RAND_MAX); // f will be between 0 and 1 not inclusive
	while (f == 1 || f == 0) // ensure that f is not exactly 0 or 1
		f = (double)rand() / (RAND_MAX / 1.0 + 1.0);

	double intervalTime = -1 * (1.0 / avg) * log(f);
	return intervalTime;
}

void processNextEvent(pqP pqPtr, queueP qPtr, custP customer, int mu, int M){
	if (customer->departureTime == -1){ // event is an arrival
		if (pqPtr->serverAvailableCount > 0){ // and a server is available
			pqPtr->serverAvailableCount--; // decrement server
			customer->startOfServiceTime = customer->arrivalTime; // update times
			customer->departureTime = customer->arrivalTime + getNextRandInterval(mu); // generate random service interval
			customer->PQtime = customer->departureTime;
			enPQ(pqPtr, customer); // place in PQ
		}
		else // customer goes into FIFO queue
			enQueue(qPtr, customer);
	}
	else{ // event is a departure
		// increment server available count
		pqPtr->serverAvailableCount++;

		// process stats for departure
		pqPtr->totalServiceTime += customer->departureTime - customer->startOfServiceTime;; // update total service time
		pqPtr->totalWaitTime += customer->startOfServiceTime - customer->arrivalTime; // update total wait time

		if (pqPtr->serverAvailableCount == M) // if idle time, no one in the system
			pqPtr->Po_sim += pqPtr->customerPQ[1]->arrivalTime - customer->departureTime; // next arrival minus current departure

		// check if FIFO queue is empty
		custP fifoCust = deQueue(qPtr);
		if (fifoCust != NULL){ // customer in FIFO queue; i.e. a customer was waiting for service
			pqPtr->numCustomerWaiting_sim++;
			fifoCust->startOfServiceTime = customer->departureTime; //  service the customer in FIFO queue
			fifoCust->departureTime = fifoCust->startOfServiceTime + getNextRandInterval(mu); // random service interval
			fifoCust->PQtime = fifoCust->departureTime;
			enPQ(pqPtr, fifoCust); // place FIFO queue customer in PQ as a departure
			pqPtr->serverAvailableCount--;
		}

		// free customer after they have departed PQ and been processed
		free(customer);
	}
}

void computeStatsPrint(int lambda, int mu, int M, int totalCustomers, double finalPQTime, pqP PQ){
	// calculate analytical measures
	double Po = PoEq(lambda, mu, M);
	double L = LEq(lambda, mu, M, Po);
	double W = WEq(L, lambda);
	double Lq = LqEq(L, lambda, mu);
	double Wq = WqEq(Lq, lambda);
	double rho = rhoEq(lambda, M, mu);

	// calculate simulated measures
	PQ->Po_sim /= finalPQTime;
	PQ->L_sim = LEq(lambda, mu, M, PQ->Po_sim);
	PQ->Lq_sim = LqEq(PQ->L_sim, lambda, mu);
	PQ->W_sim = (PQ->totalWaitTime + PQ->totalServiceTime) / totalCustomers;
	PQ->Wq_sim = PQ->totalWaitTime  / totalCustomers;
	PQ->rho_sim = PQ->totalServiceTime / (M * finalPQTime);

	printf("Analytical Model\t\tSimulated Model\n");
	printf("Po = %f\t\t\tPo = %lf\n", Po, PQ->Po_sim); // (1) simulate
	printf("L = %f\t\t\tL = n/a\n", L); // (2)
	printf("W = %f\t\t\tW = %lf\n", W, PQ->W_sim); // (3) simulate
	printf("Lq = %f\t\t\tLq = n/a\n", Lq); // (4)
	printf("Wq = %f\t\t\tWq = %lf\n", Wq, PQ->Wq_sim); // (5) simulate
	printf("rho = %f\t\t\trho = %lf\n", rho, PQ->rho_sim); // (6) simulate
	printf("P(wait for service) = n/a\tP(wait for service) = %lf\n\n", PQ->numCustomerWaiting_sim / totalCustomers);
}


double PoEq(double lambda, double mu, double M){
	int i;
	double den1 = 0, den2;

	for (i = 0; i <= M - 1; i++){
		den1 += ((1.0 / factorial(i)) * pow(lambda / mu, i));
	}

	den2 = (1.0 / factorial(M))* (pow(lambda / mu, M)) * ((M * mu) / (M*mu - lambda));

	return 1.0 / (den1 + den2);
}

double LEq(double lambda, double mu, double M, double Po){
	double num = (lambda * mu) * pow(lambda / mu, M) * Po;
	double den = factorial(M - 1) * pow(M*mu - lambda, 2);
	return num / den + (lambda / mu);
}


double WEq(double L, double lambda){
	return L / lambda;
}

double LqEq(double L, double lambda, double mu){
	return L - (lambda / mu);
}

double WqEq(double Lq, double lambda){
	return Lq / lambda;
}

double rhoEq(double lambda, double M, double mu){
	return lambda / (M * mu);
}

unsigned long factorial(double f)
{
	int i;
	unsigned long result = 1;

	for (i = 1; i <= f; i++)
		result = result * i;

	return result;
}
