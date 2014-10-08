Name: Stephen Porter
Updated: 10/07/2014
Assignment: Project 1 (Monte-Carlo Simulation)

==============
Compile / Run
==============
To compile:
make

To run:
mpiexec -n <processors> pmcpi <iterations> <seed> [checkpoint interval]

Note: Do not run with the -l option if you intend to use the checkup interval option. I use an inline loading bar and the -l option will print a big fat [0] in the middle of it. I mean, if you want to see the [0] in the loading bar, all the power to you :)

======
Files
======
	1. Makefile - To build the program
	2. spmd_monte_carlo_pi.c - The code for the program
	3. pmcpi.pbs - The pbs batch script
	4. README.txt - This README file

========
Results
========
Iterations		Serial	p = 2	p = 4	p = 8	p = 16	p = 32
1000000000		18.69	8.26	4.2		2.16	1.1		0.58
2000000000		36.06	16.46	8.28	4.17	2.19	1.14
4000000000		73.12	33.13	16.58	8.3		4.31	2.22
8000000000		143.04	72.51	33.16	16.6	8.67	4.45


===========
Discussion
===========
The coding for this assignment was relatively easy and straightforward. Basically it involved taking the spmd_sum_3 program and using it as a template it to code the monte-carlo simulation and ensuring proper process managment throughout.

I decided to be cool and add a loading bar style of progress checking because I don't like having the status print out over several lines.

The hard part of this assignment, however, was the pbs scripts. No matter what I did I could not get the pbs scripts to work for the longest time. I could get batch job inserted using qsub, however, it would keep saying I reached my limit and never run it, no matter what I did. I was finally able to get a script submitted to execute, not sure what the issue was.