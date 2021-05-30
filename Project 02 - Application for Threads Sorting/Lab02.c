#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

/***** GLOBAL VARIABLES *****/

// Mutex variable to control which thread can access to the shared variables
pthread_mutex_t mutex; 

// An original array - an provided array from user
int* unsortedArray;

// An sorted array
int* sortedArray;

// Size of an array
int arraySize;

// Index of the Threads
int threadID = 0;

/***** FUNCTIONS *****/
void* sorter(void *params);
void* merger(void* params);

/***** MAIN FUNCTION *****/
int main(int argc, char* argv[])
{	
	/***** INPUT *****/
	// Get the size of an array
	printf("Enter the size of an array: ");
	scanf("%d", &arraySize);
	
	// Generate an unsortedArray and sortedArray
	unsortedArray = (int*)calloc(arraySize, sizeof(int));
	sortedArray = (int*)calloc(arraySize, sizeof(int));
	
	// Get an array from user
	printf("Enter an array: ");
	for (int i = 0; i < arraySize; i++)
		scanf("%d", &unsortedArray[i]);
	
	/***** CREATE THREADS *****/
	// Create 3 threads in total: 2 sorting threads and 1 merging threads
	pthread_t threads[3];
	// Create a mutex
	pthread_mutex_init(&mutex, NULL);
	
	/***** SPLIT AN ARRAY INTO 2 PARTS THEN SORT *****/
	for (int i = 0; i < 2; i++)
		pthread_create(&threads[i], NULL, sorter, (void*) unsortedArray);
		
	for (int i = 0; i < 2; i++)
		pthread_join(threads[i], NULL);

	/***** MERGE 2 PARTS INTO ONE ARRAY *****/
	pthread_create(&threads[3], NULL, merger, (void*) unsortedArray);
	
	pthread_join(threads[3], NULL);
	
	/***** PRINT AN ARRAY AFTER SORTING *****/
	printf("AN ARRAY AFTER SORTING: ");
	for (int i = 0; i < arraySize; i++)
		printf("%d ", sortedArray[i]);
	printf("\n");
	
	/***** EXIT THREADS & DESTROY MUTEX *****/
	pthread_exit(NULL);
	pthread_mutex_destroy(&mutex);
	
	return EXIT_SUCCESS;
}

/***** Sorting Thread ****/
void* sorter(void *params)
/*
	+ Introduction: Slipt an array unsortedArray into 2 parts and sort each part.
	+ Input:
		* unsortedArray
		* mutex: to lock/unlock the right of accessing shared Data
		* threadID: ID of the current thread
		* arraySize: size of an array
	+ Output: unsortedArray with 2 parts are sorted.
*/
{
	// Lock a mutex
	pthread_mutex_lock(&mutex);
	
	// Initialize needed variables for this function
	int* array = (int*)params;	
	int temp, r, l;
	
	// Indetify the starting and finishing elements for each part to begin sorting
	threadID++;
	
	// Thread 1 will sort the elements from 0 to arraySize / 2 - 1
	if (threadID == 1)
	{
		l = 0; 
		r = arraySize / 2;
	}
	// Thread 2 will sort the elements from arraySize / 2 to arraySize
	else
	{
		l = arraySize / 2;
		r = arraySize;
	}
	
	// Selection Sort
	for (int i = l; i < r - 1; i++)
		for (int j = i + 1; j < r; j++)
			if (array[j] < array[i])
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
	
	// Copy the sorted part into the provided array
	for (int i = l; i < r; i++)
		unsortedArray[i] = array[i];
	
	// Release a mutex
	pthread_mutex_unlock(&mutex);
	
	// Exit thread
	pthread_exit(NULL);
}

/*****  Merging Thread *****/
void* merger(void* params)
/*
	+ Introduction: Merge two ordered sub-arrays of unsortedArray into one ascending sequence called sortedArray.
	+ Input:
		* unsortedArray
		* arraySize: size of an array
	+ Output: sortedArray
*/
{
	// Initialize needed variables for this function
	int * array = (int*)params;
	int index = 0;
	int l1 = 0, l2 = arraySize / 2, r1 = arraySize / 2, r2 = arraySize;
	
	/* Merge array: 
		(1) Go through each element in each part and compare it with the element of another part to consider with element will be written into an sortedArray; 
		(2) Do the above task until go over 2 parts;
	*/
	do
	{
		while (l1 < r1 && array[l1] <= array[l2])
		{
			sortedArray[index] = array[l1];
			l1++;
			index++;
		}
		while (l2 < r2 && array[l2] <= array[l1])
		{
			sortedArray[index] = array[l2];
			l2++;
			index++;
		}
	}
	while (l1 <r1 && l2<r2);
	
	while (l1 < r1)
	{
		sortedArray[index] = array[l1];
		index++;
		l1++;
	}
	
	while (l2 < r2)
	{
		sortedArray[index] = array[l2];
		index++;
		l2++;
	}
	
	// Exit thread
	pthread_exit(NULL);
}
