/*
 * CS 261 Assignment 5
 * Name: Charlotte Murphy
 * Date: 11/9/2016
 */

#include "dynamicArray.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define TESTING

#ifndef TESTING
static void adjustHeap(DynamicArray* heap, int last, int position, compareFunction compare);
static void buildHeap(DynamicArray* heap, compareFunction compare);
#endif

struct DynamicArray
{
    TYPE* data;
    int size;
    int capacity;
};

// --- Dynamic array ---

static void setCapacity(DynamicArray* array, int capacity)
{
    TYPE* data = malloc(sizeof(TYPE) * capacity);
    for (int i = 0; i < array->size; i++)
    {
        data[i] = array->data[i];
    }
    free(array->data);
    array->data = data;
    array->capacity = capacity;
}

static void init(DynamicArray* array, int capacity)
{
    assert(capacity > 0);
    array->data = NULL;
    array->size = 0;
    setCapacity(array, capacity);
}

DynamicArray* dyNew(int capacity)
{
    DynamicArray* array = malloc(sizeof(DynamicArray));
    init(array, capacity);
    return array;
}

void dyDelete(DynamicArray* array)
{
    free(array->data);
    free(array);
}

void dyAdd(DynamicArray* array, TYPE value)
{
    if (array->size >= array->capacity)
    {
        setCapacity(array, 2 * array->capacity);
    }
    array->data[array->size] = value;
    array->size++;
}

void dyAddAt(DynamicArray* array, TYPE value, int position)
{
    assert(position <= array->size);
    dyAdd(array, value);
    for (int i = array->size - 1; i > position; i--)
    {
        dySwap(array, i, i - 1);
    }
}

void dyPut(DynamicArray* array, TYPE value, int position)
{
    assert(position < array->size);
    array->data[position] = value;
}

void dyRemoveAt(DynamicArray* array, int position)
{
    assert(position < array->size);
    for (int i = position; i < array->size - 1; i++)
    {
        array->data[position] = array->data[position + 1];
    }
    array->size--;
}

TYPE dyGet(DynamicArray* array, int position)
{
    assert(position < array->size);
    return array->data[position];
}

int dySize(DynamicArray* array)
{
    return array->size;
}

void dySwap(DynamicArray* array, int position1, int position2)
{
    assert(position1 < array->size);
    assert(position2 < array->size);
    TYPE temp = array->data[position1];
    array->data[position1] = array->data[position2];
    array->data[position2] = temp;
}

// --- Stack ---

void dyStackPush(DynamicArray* stack, TYPE value)
{
    dyAdd(stack, value);
}

void dyStackPop(DynamicArray* stack)
{
    dyRemoveAt(stack, stack->size - 1);
}

TYPE dyStackTop(DynamicArray* stack)
{
    return dyGet(stack, stack->size - 1);
}

int dyStackIsEmpty(DynamicArray* stack)
{
    return stack->size == 0;
}

// --- Bag ---

static int findFirst(DynamicArray* array, TYPE value, compareFunction compare)
{
    for (int i = 0; i < array->size; i++)
    {
        if (compare(value, array->data[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

void dyBagAdd(DynamicArray* bag, TYPE value)
{
    dyAdd(bag, value);
}

void dyBagRemove(DynamicArray* bag, TYPE value, compareFunction compare)
{
    int position = findFirst(bag, value, compare);
    if (position != -1)
    {
        dyRemoveAt(bag, position);
    }
}

int dyBagContains(DynamicArray* bag, TYPE value, compareFunction compare)
{
    return findFirst(bag, value, compare) != -1;
}

// --- Ordered bag ---

static int binarySearch(DynamicArray* array, TYPE value, compareFunction compare)
{
    int low = 0;
    int high = array->size - 1;
    while (low <= high)
    {
        int middle = (low + high) / 2;
        if (compare(value, array->data[middle]) < 0)
        {
            high = middle - 1;
        }
        else if (compare(value, array->data[middle]) > 0)
        {
            low = middle + 1;
        }
        else
        {
            return middle;
        }
    }
    return low;
}

void dyOrderedAdd(DynamicArray* bag, TYPE value, compareFunction compare)
{
    int position = binarySearch(bag, value, compare);
    dyAddAt(bag,value, position);
}

void dyOrderedRemove(DynamicArray* bag, TYPE value, compareFunction compare)
{
    int position = binarySearch(bag, value, compare);
    if (compare(value, bag->data[position]) == 0)
    {
        dyRemoveAt(bag, position);
    }
}

int dyOrderedContains(DynamicArray* bag, TYPE value, compareFunction compare)
{
    int position = binarySearch(bag, value, compare);
    return compare(value, dyGet(bag, position)) == 0;
}

// --- Heap ---

/**
 * Adjusts heap to maintain the heap property.
 * @param heap
 * @param last  index to percolate down to.
 * @param position  index where adjustment starts.
 * @param compare  pointer to compare function.
 */
void adjustHeap(DynamicArray* heap, int last, int position, compareFunction compare)
{
	// FIXME: implement
	assert(heap != NULL);
	int parent = position;
	int leftChild = (2 * position) + 1; 
	int rightChild = (2 * position) + 2;
	
	if (rightChild < last) { /*two children */
		/*get index of smallest child*/
		int smallestChild;

		if (compare(heap->data[leftChild], heap->data[rightChild]) == -1) {
			smallestChild = leftChild;
		}
		else {
			smallestChild = rightChild;
		}

		/*if value at smallestChild < value at parent, swap nodes*/
		if (compare(heap->data[smallestChild], heap->data[parent]) == -1) {
			dySwap(heap, parent, smallestChild);
			adjustHeap(heap, last, smallestChild, compare);
		}
	}
	else if (leftChild < last) { /*one child */

		 /*if value at leftChild < value at pos, swap nodes*/
		if (compare(heap->data[leftChild], heap->data[parent]) == -1) {
			dySwap(heap, parent, leftChild);
			adjustHeap(heap, last, leftChild, compare);
		}
	}
}

/**
 * Builds a valid heap from an arbitrary array.
 * @param heap  array with elements in any order.
 * @param compare  pointer to compare function.
 */
void buildHeap(DynamicArray* heap, compareFunction compare) {
    // FIXME: implement
	assert(heap != NULL);

	int max = dySize(heap);
	int i = (max / 2) - 1;
	
	do {
		adjustHeap(heap, max, i, compare);
		i -= 1;
	} while (i > -1);
}

/**
 * Adds an element to the heap.
 * @param heap
 * @param value  value to be added to heap.
 * @param compare  pointer to compare function.
 */
void dyHeapAdd(DynamicArray* heap, TYPE value, compareFunction compare)
{
    // FIXME: implement
	int pos = dySize(heap);
	int parent;

	/*add value to first available position in heap*/
	dyAdd(heap, value);

	/*compare new node to all nodes between it and head then swap if new node is less than*/
	while (pos != 0) {
		parent = (pos - 1) / 2;
		if (compare(dyGet(heap, pos), dyGet(heap, parent)) == -1) {
			dySwap(heap, parent, pos);
			pos = parent;
		}
		else {
			return;
		}
	}
}

/**
 * Removes the first element, which has the min priority, form the heap.
 * @param heap
 * @param compare  pointer to the compare function.
 */
void dyHeapRemoveMin(DynamicArray* heap, compareFunction compare)
{
    // FIXME: implement
	assert(heap != NULL);
	assert(dySize(heap) > 0);

	int last;
	last = dySize(heap) - 1;

	/*copy value at last to head and remove last then adjust*/
	dyPut(heap, dyGet(heap, last), 0);
	//dySwap(heap, last, 0);
	dyRemoveAt(heap, last);
	adjustHeap(heap, last, 0, compare);

}

/**
 * Returns the first element, which has the min priority, from the heap.
 * @param heap
 * @return  Element at the top of the heap.
 */
TYPE dyHeapGetMin(DynamicArray* heap)
{
    // FIXME: implement
	/*return value at head*/
    return heap->data[0];
}

/**
 * Sorts arbitrary array in-place.
 * @param heap  array with elements in arbitrary order.
 * @param compare  pointer to the compare function.
 */
void dyHeapSort(DynamicArray* heap, compareFunction compare) 
{
    // FIXME: implement
	int last = dySize(heap) - 1;

	/*change the dynamic array passed as heap into a proper heap*/
	buildHeap(heap, compare);
	
	/*for i = last to i = 0, swap first & last elements, adjust heap from first to last, decrement last*/
	for (int i = last; i > 0; i--) {
		dySwap(heap, 0, i);
		adjustHeap(heap, i, 0, compare);
	}
}

// --- Iterator ---

DynamicArrayIterator* dyIteratorNew(DynamicArray* array)
{
    DynamicArrayIterator* iterator = malloc(sizeof(DynamicArrayIterator));
    iterator->array = array;
    iterator->current = 0;
    return iterator;
}

void dyIteratorDelete(DynamicArrayIterator* iterator)
{
    free(iterator);
}

int dyIteratorHasNext(DynamicArrayIterator* iterator)
{
    return iterator->current < iterator->array->size;
}

TYPE dyIteratorNext(DynamicArrayIterator* iterator)
{
    TYPE value = dyGet(iterator->array, iterator->current);
    iterator->current++;
    return value;
}

void dyIteratorRemove(DynamicArrayIterator* iterator)
{
    iterator->current--;
    dyRemoveAt(iterator->array, iterator->current);
}

// --- Utility ---

void dyPrint(DynamicArray* array, printFunction print)
{
    printf("\nsize: %d\ncapacity: %d\n[\n", array->size, array->capacity);
    for (int i = 0; i < array->size; i++)
    {
        printf("%d: ", i);
        print(array->data[i]);
        printf(", ");
    }
    printf("]\n");
}

void dyCopy(DynamicArray* source, DynamicArray* destination)
{
    free(destination->data);
    init(destination, source->capacity);
    for (int i = 0; i < source->size; i++)
    {
        destination->data[i] = source->data[i];
    }
    destination->size = source->size;
}