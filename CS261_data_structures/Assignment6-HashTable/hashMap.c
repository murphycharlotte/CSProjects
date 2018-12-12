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
#include <assert.h>

int hashFunction1(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += key[i];
    }
	return r;
}

int hashFunction2(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}

/**
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
HashLink* hashLinkNew(const char* key, int value, HashLink* next)
{
    HashLink* link = malloc(sizeof(HashLink));
    link->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(link->key, key);
    link->value = value;
    link->next = next;
    return link;
}

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(HashLink* link)
{
    free(link->key);
    free(link);
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapInit(HashMap* map, int capacity)
{
    map->capacity = capacity;
    map->size = 0;
    map->table = malloc(sizeof(HashLink*) * capacity);
    for (int i = 0; i < capacity; i++)
    {
        map->table[i] = NULL;
    }
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(HashMap* map) {
    // FIXME: implement
	assert(map != 0);

	HashLink* tempLink;
	HashLink* nextLink;
	//for each bucket, delete all links 
	for (int i = 0; i < map->capacity; i++) {
		tempLink = map->table[i];
		//iterate through each linked list in table
		while (tempLink != NULL) {
			nextLink = tempLink->next;
			hashLinkDelete(tempLink);
			map->size--;
			tempLink = nextLink;
		}
		map->table[i] = NULL;
	}
}

/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
HashMap* hashMapNew(int capacity)
{
    HashMap* map = malloc(sizeof(HashMap));
    hashMapInit(map, capacity);
    return map;
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(HashMap* map)
{
    hashMapCleanUp(map);
    free(map);
}

/**
 * Returns a pointer to the value of the link with the given key. Returns NULL
 * if no link with that key is in the table.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(HashMap* map, const char* key) {
    // FIXME: implement
	assert(map != 0);

	/*find correct index of linked list bucket*/
	int hashIndex = HASH_FUNCTION(key) % map->capacity;
	if (hashIndex < 0) {
		hashIndex += map->capacity;
	}

	/*first link in bucket*/
	HashLink* tempLink = map->table[hashIndex];

	/*iterate through list to find key and return address of the value pair*/
	while(tempLink != NULL) {
		if (strcmp(tempLink->key, key) == 0) {
			return &(tempLink->value); 
		} else {
			tempLink = tempLink->next;
		}
	}

	/*returns NULL if key was not found*/
    return NULL;
}

/**
 * Resizes the hash table to have a number of buckets equal to the given
 * capacity. After allocating the new table, all of the links need to be
 * rehashed into it because the capacity has changed.
 * 
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 * 
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{
    // FIXME: implement
	assert(map != 0);
	int oldCapacity = map->capacity;
	/*stores old table*/
	HashLink** tempTable = map->table;
	HashLink* tempLink;
	HashLink* tempNext;

	/*creates a new table within the existing map*/
	hashMapInit(map, oldCapacity * 2);
	/*add existing links to new buckets*/
	for (int i = 0; i < oldCapacity; i++) {
		tempLink = tempTable[i];
		while (tempLink != NULL) {
			/*track nextLink*/
			tempNext = tempLink->next;

			/*find hash index for new table capacity*/
			int hashIndex = HASH_FUNCTION(tempLink->key) % map->capacity;
			if (hashIndex < 0) {
				hashIndex += map->capacity;
			}

			/*add link to <start> of linked list at table[hashIdx]
			*set tempLink next pointer to hashIdx address
			*then hashIdx address to tempLink
			*/
			tempLink->next = map->table[hashIndex];
			map->table[hashIndex] = tempLink; /*add link to bucket*/
			map->size++;
			/*continue through list at tempTable[i]*/
			tempLink = tempNext;
		}
	}
	/*The only new memory allocated was for a new table
	*no other calls to free() needed
	*/
	free(tempTable);
}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(HashMap* map, const char* key, int value)
{
    // FIXME: implement
	//if key is found, update value, else add a new link
	int* valuePtr = NULL;
	valuePtr = hashMapGet(map, key);

	int hashIndex = HASH_FUNCTION(key) % map->capacity;
	if (hashIndex < 0) {
		hashIndex += map->capacity;
	}
	/*if key is already in the map, update the value*/
	if (valuePtr != NULL) {
		*valuePtr = value; /*update value stored at address returned by hashMapGet*/

		/* HashLink* tempLink = map->table[hashIndex];
		HashLink* tempNext;
		while (tempLink != NULL) {
			tempNext = tempLink->next;
			if (tempLink->key == key) {
				tempLink->value = *valuePtr; //update value
				tempLink = NULL;
			} else {
				tempLink = tempNext;
			}
		} 
		*/
	/*else create a new link with the given key and value*/
	} else { 
		HashLink* newNext = map->table[hashIndex];
		HashLink* newLink = hashLinkNew(key, value, newNext);
		map->table[hashIndex] = newLink;
		map->size++;
		if ((map->size / map->capacity) > 10) {
			resizeTable(map, 2 * map->capacity);
		}
	}
}

/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 */
void hashMapRemove(HashMap* map, const char* key)
{
    // FIXME: implement
	int hashIndex = HASH_FUNCTION(key) % map->capacity;
	if (hashIndex < 0) {
		hashIndex += map->capacity;
	}
	HashLink* temp = map->table[hashIndex];

	if (temp != NULL) {
		if (strcmp(temp->key, key) == 0) {
			map->table[hashIndex] = temp->next;
			hashLinkDelete(temp);
			map->size--;
			temp = NULL;
		} else {
			while (temp != NULL) {
				if (temp->next != NULL) {
					if (strcmp(temp->next->key, key) == 0) {
						HashLink* temp2 = temp->next;
						temp->next = temp2->next;
						hashLinkDelete(temp2);
						map->size--;
						temp = NULL;
					} else {
						temp = temp->next;
					}
				} else {
					temp = NULL;
				}
			}			
		}
	} 
}

/**
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 * 
 * @param map
 * @param key
 * @return 1 if the key is found, 0 otherwise.
 */
int hashMapContainsKey(HashMap* map, const char* key)
{
	// FIXME: implement
	assert(map != 0);
	HashLink *tempLink;

	//find correct index of linked list bucket
	int hashIndex = HASH_FUNCTION(key) % map->capacity;
	if (hashIndex < 0) {
		hashIndex += map->capacity;
	}

	tempLink = map->table[hashIndex];
	while (tempLink != NULL) {
		if (strcmp(tempLink->key, key) == 0) {
			return 1;
		} else {
			tempLink = tempLink->next;
		}
	}
	return 0;
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
    // FIXME: implement
    return map->size;
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map)
{
    // FIXME: implement
    return map->capacity;
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map)
{
    // FIXME: implement
	int emptyCount = 0;
	for (int i = 0; i < map->capacity; i++) {
		if (map->table[i] == NULL) {
			emptyCount++;
		}
	}
    return emptyCount;
}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(HashMap* map)
{
    // FIXME: implement
	float load = (float)map->size / map->capacity;
    return load;
}


//Prints all the links in each of the buckets in the table.
//@param map
 
void hashMapPrint(HashMap* map)
{
    for (int i = 0; i < map->capacity; i++)
    {
        HashLink* link = map->table[i];
        if (link != NULL)
        {
            printf("\nBucket %i ->", i);
            while (link != NULL)
            {
                printf(" (%s, %d) ->", link->key, link->value);
                link = link->next;
            }
        }
    }
    printf("\n");
}