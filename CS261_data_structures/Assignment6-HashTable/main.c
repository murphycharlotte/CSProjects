/*
* CS 261 Data Structures
* Assignment 6
* Name: Charlotte Murphy
* Date: 11/20/2016
*/

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Prints the concordance of the given file and performance information. Uses
 * the file input1.txt by default or a file name specified as a command line
 * argument.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    // FIXME: implement
    const char* fileName = "input1.txt";
    if (argc > 1)
    {
        fileName = argv[1];
    }
    printf("Opening file: %s\n", fileName);
    
    clock_t timer = clock();
    
    HashMap* map = hashMapNew(10);
    // --- Concordance code begins here ---
    // Be sure to free the word after you are done with it here.
    // --- Concordance code ends here ---
	/*open file for reading only*/
	FILE *file = fopen(fileName, "r");
	assert(file != NULL);
	
	char* tempWord = nextWord(file);
	/*add each word to the hash tabel*/
	while (tempWord != NULL) {
		/*if the word is found in the table, increase its count*/
		int* count = hashMapGet(map, tempWord);
		if (count != NULL) {
			*count += 1;	
		/*else add it to the table with a count of 1*/
		} else {
			hashMapPut(map, tempWord, 1);
		}
		/*free current word*/
		free(tempWord);
		/*get next word*/
		tempWord = nextWord(file);
	}
	/*close the file*/
	fclose(file);

	for (int i = 0; i < map->capacity; i++) {
		HashLink* tempLink = map->table[i];
		while (tempLink != NULL) {
			printf("%s: %d\n", tempLink->key, tempLink->value);
			tempLink = tempLink->next;
		}
	}
    hashMapPrint(map);
    
    timer = clock() - timer;
    printf("\nRan in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    printf("Empty buckets: %d\n", hashMapEmptyBuckets(map));
    printf("Number of links: %d\n", hashMapSize(map));
    printf("Number of buckets: %d\n", hashMapCapacity(map));
    printf("Table load: %f\n\n", hashMapTableLoad(map));
    
    hashMapDelete(map);
    return 0;
}