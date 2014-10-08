#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <prand.h>
#include <mpi.h>

/* Preprocessor for interval checkup */
void   print_state(int);

const int MONTE_CARLO = 1;
const int BAR_WIDTH = 50; //For the inline progress bar

/* Instance data */
static long long n, count;
static int interval = -1; 
static int id, share;

/**
 * Used to setup the prand stuff to make sure we get
 * the same random numbers as the serial using the 
 * same seed.
 */
void p_rand(int seed)
{
	long long int startingPosition = 0;

	srandom(seed);
	startingPosition = id * share;
	unrankRand(startingPosition);
}

/** 
 * The method we use to estimate pi using the 
 * monte-carlo simulation
 */
int estimate_pi(int nproc, unsigned int seed)
{
	/* Initial data */
	long long  m = 0;
	double x, y;
	int itr;
	
	/* Setup the prand library stuff */
	p_rand(seed);

	/* Run the monte-carlo simulation stuff */
	for (itr=0; itr<share; itr++) {
		count++;
		x = -1 +  (2.0 * (random() / (RAND_MAX + 1.0)));
		y = -1 +  (2.0 * (random() / (RAND_MAX + 1.0)));
		
		/* check if (x,y) is within the unit circle */
		if ((x*x + y*y) < 1)
			m++;
	}
	
	return m;
}

/** 
 * Main insertion 
 */
int main(int argc, char **argv)
{
	/* Initial data */
	int nproc, tempsum, i;
	unsigned int seed;
	double startTime, totalTime;
    double pi = 0;
    MPI_Status status;

	/* Process user command line arguments */
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <number of iterations> <random seed> [checkpoint interval]\n", argv[0]);
		exit(1);
	}

	n = atoll(argv[1]);
	seed = atoi(argv[2]);
	
	/* I made the interval checkup optional */
	if (argc == 4) {
		interval = atoi(argv[3]);
		signal(SIGALRM, print_state);
		alarm(interval);
		printf("0%%\r");
	}

  	/* Start up MPI */
  	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	/* Get our load sizes */
	share = n/nproc;
	if (id == nproc-1) share += n%nproc;
	
	/* And we're off! */
	startTime = MPI_Wtime();

  	int result = estimate_pi(nproc, seed);

	/* If not p0, send to p0 */
	if(id != 0)
		MPI_Send(&result, 1, MPI_LONG, 0, MONTE_CARLO, MPI_COMM_WORLD );

	/* We are p0 */
	else {
		/* p0's contribution */
		pi += (4 * (double)result / n);
		
		/* Get everyone else's contribution */
  		for (i=0; i<nproc-1; i++) {
			MPI_Recv(&tempsum, 1, MPI_LONG, MPI_ANY_SOURCE, MONTE_CARLO, MPI_COMM_WORLD, &status);
            pi += (4 * (double)tempsum / n);
    	}
			
		/* And we're done! */
		totalTime = MPI_Wtime() - startTime;
		
		/* Pretty printing stuff */
		if(interval > 0 && interval < 1000){
			//printf("[");

			//for(i=0; i<BAR_WIDTH-1; i++){
			//	printf("=");
			//}
			
			printf("100.0%%\n");
		}
		
		/* The results from all our hard work */
		printf(" Pi  = %f\nTime = %lfs\n", pi, totalTime);
	}

  	/* Program finished. Exit MPI */
  	MPI_Finalize();
	
	/* Good clean exit */
  	exit(0);
}

/**
 * If the checkup interval option is used this code is
 * executed every interval seconds to display a progress
 * bar
 */
void print_state(int signo)
{
	/* Initial data */
	//int curr;
	double progress = (double)count/share;
	//int pos = BAR_WIDTH * progress;
	
	/* Only print the status if we are p0 to avoid multiple prints */
	if(id == 0){
		//printf("[");
	
		//for(curr=0; curr<BAR_WIDTH-1; curr++){
		//	if(curr <= pos) printf("=");
		//	else printf(" ");
		//}
		
		printf("%04.1lf%%\r", progress*100);
		fflush(NULL);
		alarm(interval);
	}
}

