# Event-Driven Queue Simulation
C implementation of a queueing system using an event driven simulation methodology. Program uses a priority queue as the event list and a linked list to simulate server lines.

## Instructions
The program takes runSimulation.txt as the input. The following arguments must be on new lines:  
**lambda** - negative exponential distribution hyperparameter  
**mu** - negative exponential distribution hyperparameter  
**M** - total number of servers  
**n** - number of customers  

## Output
The output of the simulation compares the analytical model with the simulated model on the following metrics:
**Po** - percent idle time  
**L** - average number of customers in system  
**W** - average time a customer spends in the system  
**Lq** - average number of customers in queue  
**Wq** - average time customer spends waiting in queue  
**rho** - utilization factor of the system  
**P(wait for service time)** - probability that a customer must wait for service

