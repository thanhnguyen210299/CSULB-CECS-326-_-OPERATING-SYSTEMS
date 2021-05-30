#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

// MACRO (These functions will be called only if Macro is defined)
#ifdef PTHREAD_SYNC
	pthread_mutex_t mutex;
	pthread_barrier_t barrier;
#endif

int SharedVariable = 0; // is shared by threads

void SimpleThread(int which)
/*
	This is the provided function. The purpose of this function is 
		seeing which thread is called at which time.
		
	Input: The ID of the thread
*/
{
	int num, val;
	
	#ifdef PTHREAD_SYNC
		// Lock a mutex object
		pthread_mutex_lock(&mutex);
	#endif
	for (num = 0; num < 20; num++)
	{
		if (random() > RAND_MAX / 2)
			usleep(500);
		val = SharedVariable;
		printf("*** thread %d sees value %d\n", which, val);
		SharedVariable = val + 1;
	}
	#ifdef PTHREAD_SYNC
		// Release a mutex object
		pthread_mutex_unlock(&mutex);
		// Allow all threads to wait for the last thread to exit 				the loop.
    		pthread_barrier_wait(&barrier);
  	#endif
	val = SharedVariable;
	printf("Thread %d sees final value %d\n", which, val);
}

void* singleThread(void* threadID)
/*	This is the subfunction that is executed by the thread. The return 			type and parameter type of the subfunction must be of type 			void* because the pointer to this subfunction should be 			passed to the function thread_create()
*/
{
	SimpleThread((intptr_t)(threadID));
	
	// Terminate the thread
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{	
	// Check if the input is correct: output_file number_of_threads
	if (argc != 2)
	{
		printf("ERROR: usage: output <number of threads>\n");
		return EXIT_FAILURE;
	}
	
	// The input is correct: argc == 2
	// Convert the number_of_thread from string to number
	int numberOfThreads = atoi(argv[1]);
		
	// Check if the number of thread is positive (greater than 			0)	
	if (numberOfThreads <= 0)
	{
		printf("ERROR: usage: output <number of threads>\n");
		return EXIT_FAILURE;
	}
      	
	pthread_t threads[numberOfThreads];
	
	#ifdef PTHREAD_SYNC
		// Initialize the mutex
		pthread_mutex_init(&mutex, NULL);
		// Initialize the barrier
        pthread_barrier_init(&barrier, NULL, numberOfThreads);
    #endif
	
	// Create threads
	for (long i = 0; i < numberOfThreads; i++)
		pthread_create(&threads[i], NULL, singleThread, (void*) i);
		
	// Wait for the thread to exit
	for (long i = 0; i < numberOfThreads; i++)
		pthread_join(threads[i], NULL);
	
	// Exit threads
	pthread_exit(NULL);
					
	return EXIT_SUCCESS;
}
