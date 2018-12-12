/*
* CS 261 Assignment 5
* Name: Charlotte Murphy
* Date: 11/9/2016
*/

#include "task.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/**
 * Creates a new task with the given priority and name.
 * @param priority
 * @param name
 * @return  The new task.
 */
Task* taskNew(int priority, char* name)
{
    // FIXME: implement
	Task* newTask = (Task*)malloc(sizeof(Task));
	assert(newTask != 0);

	newTask->priority = priority;
	strcpy(newTask->name, name);
    return newTask;
}

/**
 * Frees a dynamically allocated task.
 * @param task
 */
void taskDelete(Task* task)
{
    free(task);
}

/**
 * Casts left and right to Task pointers and returns
 * -1 if left's priority < right's priority,
 *  1 if left's priority > right's priority,
 *  0 if left's priority == right's priority.
 * @param left  Task pointer.
 * @param right  Task pointer.
 * @return 
 */
int taskCompare(void* left, void* right)
{
    // FIXME: implement
	assert(left != NULL);
	assert(right != NULL);

	Task* tempLeft = (Task*)left;
	Task* tempRight = (Task*)right;

	if (tempLeft->priority < tempRight->priority) {
		return -1;
	}
	else if (tempLeft->priority > tempRight->priority) {
		return 1;
	}
	else { /*priorities are equal*/
		return 0;
	}
}

/**
 * Prints a task as a (priority, name) pair.
 * @param value  Task pointer.
 */
void taskPrint(void* value)
{
    Task* task = (Task*)value;
	printf("(%d, %s)", task->priority, task->name);
}

/* Sources
http://stackoverflow.com/questions/27707689/error-incompatible-types-when-assigning-to-type-char25-from-type-char
*/