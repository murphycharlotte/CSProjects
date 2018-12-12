/*
* CS 261 Assignment 5
* Name: Charlotte Murphy
* Date: 11/9/2016
*/

#include "dynamicArray.h"
#include "task.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

/**
 * Loads into heap a list from a file with lines formatted like
 * "priority, name".
 * @param heap
 * @param file
 */
void listLoad(DynamicArray* heap, FILE* file)
{
    const int FORMAT_LENGTH = 256;
    char format[FORMAT_LENGTH];
    snprintf(format, FORMAT_LENGTH, "%%d, %%%d[^\n]", TASK_NAME_SIZE);
    
    Task temp;
    while (fscanf(file, format, &temp.priority, &temp.name) == 2)
    {
        dyHeapAdd(heap, taskNew(temp.priority, temp.name), taskCompare);
    }
}

/**
 * Writes to a file all tasks in heap with lines formatted like
 * "priority, name".
 * @param heap
 * @param file
 */
void listSave(DynamicArray* heap, FILE* file)
{
    for (int i = 0; i < dySize(heap); i++)
    {
        Task* task = dyGet(heap, i);
        fprintf(file, "%d, %s\n", task->priority, task->name);
    }
}

/**
 * Prints every task in heap.
 * @param heap
 */
void listPrint(DynamicArray* heap)
{
    DynamicArray* temp = dyNew(1);
    dyCopy(heap, temp);
    while (dySize(temp) > 0)
    {
        Task* task = dyHeapGetMin(temp);
        printf("\n");
        taskPrint(task);
        printf("\n");
        dyHeapRemoveMin(temp, taskCompare);
    }
    dyDelete(temp);
}

/**
 * Handles the given command.
 * @param list
 * @param command
 */
void handleCommand(DynamicArray* list, char command)
{
    // FIXME: Implement
	assert(list != NULL);
	char cmd = tolower(command);
	switch(cmd) 
	{
		case 'l':		//"'l' to load to-do list from a file\n"
		{
			/*get file name*/
			char fileName[100];		
			printf("Please enter the file name: ");
			/*read characters until '/n'. '%*c' eats the newline char*/
			scanf("%[^\n]%*c", fileName);
			/*create file pointer and open file for reading only*/
			FILE* lFile = fopen(fileName, "r");
			/*checks if file exists and is accessable*/
			if (!lFile)
			{
				printf("File does not exist or you do not have access\n\n");
				return;
			}
			listLoad(list, lFile);
			fclose(lFile);
			printf("The list has been loaded from file successfully\n\n");
			return;
		}
		case 's':		//"'s' to save to-do list to a file\n"
		{
			/*get file name*/
			char fileName[100];
			printf("Please enter the file name: ");
			/*read characters until '/n'. '%*c' eats the newline char*/
			scanf("%[^\n]%*c", fileName);
			/*create file pointer and opens file for writing from the beginning of the file*/
			/*creates new file, if necessary*/
			FILE* sFile = fopen(fileName, "w");
			listSave(list, sFile);
			fclose(sFile);
			printf("The list has been saved to the file successfully\n\n");
			return;		
		}
		case 'a':		//"'a' to add a new task\n"
		{
			char description[100];
			int priority;
			printf("Please enter the task description: ");
				scanf("%[^\n]%*c", description);
			printf("Please enter the task priority (0-999): ");
				scanf("%d%*c", &priority);
				while (priority > 999 || priority < 0)
				{
					printf("Please enter an integer between 0-999:\n");
					scanf("%d%*c", &priority);
				}
			Task* newTask = taskNew(priority, description);
			dyHeapAdd(list, newTask, taskCompare);
			printf("The task '%s' has been added to your todo list.\n\n", description);
			return;
		}
		case 'g':		//"'g' to get the first task\n"
		{
			if (dySize(list) < 1) 
			{
				printf("Your to-do list is empty!\n\n");
				return;
			}
			else 
			{
				printf("Your first task is: ");
				taskPrint(dyHeapGetMin(list));
				printf("\n\n");
				return;
			}
				
		}
		case 'r':		//"'r' to remove the first task\n"
		{
			if (dySize(list) > 0) 
			{
				Task* first = dyHeapGetMin(list);
				dyHeapRemoveMin(list, taskCompare);
				printf("Your first task '%s' has been removed from your todo list.\n\n", first->name);
				first = NULL;
				return;
			}
			else 
			{
				printf("Your to-do list is empty!\n\n");
				return;
			}
		}
		case 'p':		//"'p' to print the list\n"
		{
			if (dySize(list) < 1) 
			{
				printf("Your to-do list is empty!\n\n");
				return;
			}
			else
			{
				listPrint(list);
				printf("\n");
				return;
			}
		}
		case 'e':		//"'e' to exit the program\n"
		{
			printf("Goodbye!\n\n");
			return;
		}
		default: 
		{
			printf("Please enter a valid option.\n\n");
			return;
		}
	}
}

		

int main()
{
    // Implement
    printf("\n\n** TO-DO LIST APPLICATION **\n\n");
    DynamicArray* list = dyNew(8);
    char command = ' ';
	//FILE toDoTxtFile = fopen("todo.txt, ")
    do
    {
        printf("Press:\n"
               "'l' to load to-do list from a file\n"
               "'s' to save to-do list to a file\n"
               "'a' to add a new task\n"
               "'g' to get the first task\n"
               "'r' to remove the first task\n"
               "'p' to print the list\n"
               "'e' to exit the program\n"
        );
        command = getchar();
        // Eat newlines
		
        while (getchar() != '\n');
        handleCommand(list, command);
    }
    while (command != 'e');
    dyDelete(list);
    return 0;
}

/* sources
	http://cboard.cprogramming.com/c-programming/28889-using-switch-statement-characters.html
	http://beginnersbook.com/2014/01/switch-case-statements-in-c/
	http://stackoverflow.com/questions/6083045/scanf-n-skips-the-2nd-input-but-n-does-not-why
	http://stackoverflow.com/questions/2795382/enter-custom-file-name-to-be-read
	http://stackoverflow.com/questions/9482770/checking-a-file-exists-in-c
	https://www.tutorialspoint.com/cprogramming/c_file_io.htm
*/