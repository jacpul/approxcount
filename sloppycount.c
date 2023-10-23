#include <ctype.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <time.h>

/* global counter value */
volatile long long int counter;

/* you only need this one lock */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* we'll pass a pointer to this structure to provide parameters
 * to the worker threads */
struct targs {
	long long int target;
	long long int slop;
};

void usage() {

	/* print out how this program is meant to be used --
	 * useful for error conditions / help screens.
	 */

	printf("sloppycount -- perform sloppy counting\n");
	printf("Usage: sloppycount -n target -s slop -t threads -m mode\n");
	printf("\t-n target\tCount up to this number.\n");
	printf("\t-t threads\tThe number of counting threads to use.\n");
	printf("\t-s slop\t\tEach thread will count up to this before synchronizing.\n");
	printf("\t-m mode\t\tSlop mode: noslop, sloppy, random1, random2\n");
	printf("\t-r reps\t\tHow many repetitions to perform.\n");
	printf("\t-d debug\t\tEnable debug mode.\n");
	printf("\n");
	printf("See manual for mode definitions.\n");
	printf("\n");

}

/* From: https://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/ 
 * Modified to convert timespec to float
 * */
float diff2float(struct timespec *start, struct timespec *end)
{
	float f;

	struct timespec temp;

	if ((end->tv_nsec - start->tv_nsec)<0) {
		temp.tv_sec = end->tv_sec - start->tv_sec-1;
		temp.tv_nsec = 1000000000 + end->tv_nsec - start->tv_nsec;
	} else {
		temp.tv_sec = end->tv_sec - start->tv_sec;
		temp.tv_nsec = end->tv_nsec - start->tv_nsec;
	}

	f = temp.tv_sec + (temp.tv_nsec / 1000000000.0);

	return f;
}


void *sloppy(void *args) {

	/* perform sloppy counting locally
	 * lock only for each increment of the global counter
	 * after each count of targs->slop values
	 */

	struct targs *targs;
	targs = (struct targs *)args;
	long long int local = 0;

	/* FIXME: put your sloppy counting code here */

}

void *random2(void *args) {

	/* perform sloppy counting locally
	 * lock only for each increment of the global counter
	 * roll a random slop value for *each* count to slop value
	 * random value should be between 1 and (2 * targs->slop)
	 */

	struct targs *targs;
	targs = (struct targs *)args;
	long long unsigned tempslop;
	long long int local = 0;

	/* FIXME: put your 'random2' sloppy code here */

}

void *noslop(void *args) {

	/* perform threadsafe counting while locking for each increment.
	 * this has no sloppiness, and in fact the slop value is ignored
	 * in this function.
	 */

	struct targs *targs;
	targs = (struct targs *)args;

	/* FIXME: put your non-sloppy counting code here */

}

int main(int argc, char *argv[]) {


	/* GET OPTIONS IN A NICE WAY
	 *
	 * Getopt code adapted from:
	 * https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
	 * FYI, there's also a getopt_long with more features...
	 */

	int option;
	long long int target = -1;
	long long int slop = -1;
	unsigned numthreads = 0;
	int reps;
	char *mode = NULL; /* default option */
	int i = 0;
	int j = 0;
	int rc = 0;
	int check = 0;
	int debug = 0;
	pthread_t *threads = NULL;
	struct targs* targs;
	int seed = time(NULL); /* get current time for seed */
	struct timespec start, end;
	float elapsed;

	opterr = 0;

	while ((option = getopt(argc, argv, "n:s:t:m:r:hcd")) != -1 )
	{
		switch(option)
		{
			case 'n':
				target = atol(optarg);
				break;

			case 's':
				slop = atol(optarg);
				break;

			case 't':
				numthreads = atoi(optarg);
				break;

			case 'm':
				mode = optarg;
				break;

			case 'r':
				reps = atoi(optarg);
				break;

			case 'h':
				usage();
				goto out; /* goto not always considered harmful! */

			case 'c':
				check = 1;
				break;

			case 'd':
				debug = 1;
				break;

			case '?':
				if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", 
							 optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", 
							 optopt);
				usage();
				abort();
			default:
				usage();
				abort();

		}

	}

	if (target == -1 || slop == -1 || numthreads == 0 || mode == NULL) {
		fprintf(stderr, "One or more required arguments were missing!\n");
		usage();
		abort();
	}

	if (debug) fprintf(stderr, "target: %lld numthreads: %d slop: %lld seed: %d mode: %s\n", 
					  target, numthreads, slop, seed, mode);

	/* initialize RNG with seed */
	srand(seed);

	/* allocate our pthread structures based on how many threads are requested */
	threads = (pthread_t *)malloc(sizeof(pthread_t) * numthreads);
	/* the first thread is at threads[0]; a pointer to that thread is &threads[0]. */

	assert(threads != NULL);

	/* allocate array of targs -- one for each thread */
	targs = (struct targs*)malloc(sizeof(struct targs) * numthreads);

	assert(targs != NULL);

	/* the outer 'reps' for loop just repeats the inner loop with the threads */
	for (reps; reps > 0; reps--) {

		/* start timing for this repetition */
		clock_gettime(CLOCK_REALTIME, &start);

		/* start the threads --
		 * we start one thread each time through this loop (up to numthreads)!
		 **/
		for (i = 0; i < numthreads; i++) {

			/* prepare this thread's targs struct */
			targs[i].target = target;
			targs[i].slop = slop;
			/* you can reference the current thread's targs as a pointer using &targs[i] 
			 * ... in each counting fuction, however, there is a local targs pointer 
			 * for that thread. */

			if (strncmp(mode, "noslop", 5) == 0) {

				rc = pthread_create(&threads[i], NULL, noslop, (void*)&targs[i]);

			} else if (strncmp(mode, "sloppy", 6) == 0) {

				assert(targs[i].slop > 0);
				
				/* FIXME: create a 'sloppy' thread here */

			} else if (strncmp(mode, "random1", 7) == 0) {

				assert(targs[i].slop > 0);

				/* for random, roll a sloppiness value between 1 and 2*slop and use it here */
				
				/* FIXME: set targs[i].slop to a random value between 1 and (2 * slop) */

				/* FIXME: create a 'random1' thread here (hint: you can use the 'sloppy' function)*/

			} else if (strncmp(mode, "random2", 7) == 0) {

				assert(targs[i].slop > 0);

				/* FIXME: create a 'random2' thread here */

			} else {

				fprintf(stderr, "Unknown mode '%s'. Quitting\n", mode);
				return(1);

			}
			assert(rc == 0);

		}

		/* join on all the threads, in order */

		/* FIXME: wait for all threads to complete before continuing */

		/*********************************************************/
		/* get time of loop end and print statistics 
		 * DON'T CHANGE THIS CODE */
		clock_gettime(CLOCK_REALTIME, &end);
		elapsed = diff2float(&start, &end);

		if (check == 1) {
			/* only print counts if -c option is used */
			printf("target:\t%lld\tcounter:\t%lld\ttime:\t", target, counter);
		}

		printf("%f\n", elapsed);

		counter = 0; /* reset counter for next repetition */
		/*********************************************************/

	}

	free(threads);
	free(targs);

out:
	return 0;

}
