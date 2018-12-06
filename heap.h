/* 
 * This module implements a binary-min-heap of integer data and float keys
 *
 * Author: James Barnes (820946)
 */

#ifndef HEAP_H
#define HEAP_H


typedef struct heap Heap;

/* malloc, initalise and return a pointer to a new heap
 *
 * MAX_SIZE MUST BE GREATER THAN ZERO */
Heap *new_heap(int max_size);

/* return the number of items stored in a heap */
int heap_count(Heap *h);

/* check if the heap is empty */
int heap_empty(Heap *h);

/* check if allocated heap size is full */
int heap_full(Heap *h);

/* insert a new item into a heap, then restore the heap property 
 * O(log n) 
 *
 * CHECK IF THERE IS SPACE IN HEAP BEFORE INSERTING */
void heap_insert(Heap *h, float key, int data);

/* remove the minimum from the heap, then restore the heap property
 * O(log n)
 *
 * CHECK IF THE HEAP IS NOT EMPTY BEFORE REMOVING MIN */
int heap_remove_min(Heap *h);

/* update the key/data of the min in heap, and update the heap accordingly
 * O(log n)
 *
 * CHECK IF THE HEAP IS NOT EMPTY BEFORE UPDATING */
void heap_update_min(Heap *h, float key, int data);

/* peek at the data of the minimum item in the heap
 *
 * CHECK IF THE HEAP IS NOT EMPTY BEFORE PEEKING */
int heap_peek_min(Heap *h);

/* peek at the data of the minimum item in the heap
 *
 * CHECK IF THE HEAP IS NOT EMPTY BEFORE PEEKING */
float heap_peek_key(Heap *h);

/* free and cleanup all memory allocated to a heap
 *
 * H MUST NOT BE NULL */
void free_heap(Heap *h);


#endif        //  #ifndef HEAP_H