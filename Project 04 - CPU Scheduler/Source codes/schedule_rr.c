/**
 * Round-robin scheduling
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "CPU.h"
#include "schedulers.h"

static struct node* taskList = NULL;
static struct node* currentNode = NULL;

// add a task to the list
void add(char* name, int priority, int burst)
{
	// create a new task based on the parameters
	Task* newTask = malloc(sizeof(Task));
	newTask->priority = priority;
	newTask->burst = burst;
	// copy the name of task to store in the list
	newTask->name = malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(newTask->name, name);

	// add the new task to list, the new task will be placed before the head
	insert(&taskList, newTask);
}

struct node* getLastNode()
{
	struct node* tail = taskList;
	
	while(tail->next)
		tail = tail->next;
	
	return tail;
}

struct node* findNextNode(Task* currentTask)
{
    // if the list only has one node
    if (!taskList->next)
    	return taskList;
    else
    {
    	struct node* temp = taskList;
	struct node* prev;
        // interior or last element in the list
        prev = temp;
        temp = temp->next;
        while (strcmp(currentTask->name,temp->task->name) != 0)
        {
            prev = temp;
            temp = temp->next;
        }

        return prev;
    }
}

// pick the next Task based on RR algorithm
Task* pickNextTask()
{
	Task* nextTask = currentNode->task;
	// if the node is not the first node in the list => currentNode goes to the next one
	if (strcmp(currentNode->task->name,taskList->task->name) != 0)
		currentNode = findNextNode(currentNode->task);
	// if the node is the first node in the list => currentNode begins at the tail
	else
		currentNode = getLastNode();
		
	return nextTask;
}

// invoke the scheduler
void schedule()
{
	int burstTime;
	currentNode = getLastNode();
	while (taskList)
	{
		Task* nextTask = pickNextTask();
		if (QUANTUM <= nextTask->burst)
			burstTime = QUANTUM;
		else
			burstTime = nextTask->burst;
		run(nextTask, burstTime);
		nextTask->burst -= burstTime;
		if (!nextTask->burst)
			delete(&taskList, nextTask);
	}
}
