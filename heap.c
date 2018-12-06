/*
 * Implemenations of various functions required for "heap.h"
 *
 * Author: James Barnes (820946)
 */

#include "heap.h"

#include <stdlib.h>
#include <assert.h>

typedef struct heapItem HeapItem;

struct heap
{
	HeapItem **items; // storage of heap items
	int max_size; // size of the heap
	int next; // first empty index
};

struct heapItem
{
	int data;
	float key;
};

/* compare the key of the parent to the item i, and swap if less then 
 * recursively sift up if swapped */
void sift_up(Heap *h, int i);

/* compare the key of the minimum child to the item i, and swap if greater 
 * then recursively sift down if swapped */
void sift_down(Heap *h, int i);

/* find the index of the child of item i with the lowest key
 * 
 * return 0 if there are no children of i */
int min_child(Heap *h, int i);

/* swap two items in a heap */
void heap_swap_items(Heap *h, int a, int b);

Heap *new_heap(int max_size)
{
	assert(max_size > 0);
	Heap *h;
	assert(h = malloc(sizeof(Heap)));
	assert(h->items = malloc(sizeof(HeapItem*) * max_size));
	h->items[0] = NULL;
	h->max_size = max_size;
	h->next = 0;
	return h;
}

int heap_count(Heap *h)
{
	return h->next;
}

int heap_empty(Heap *h)
{
	return !h->next;
}

int heap_full(Heap *h)
{
	return h->next == h->max_size;
}

void heap_insert(Heap *h, float key, int data)
{
	// check there is space to store a new item
	assert(!heap_full(h));
	
	// create the new item
	HeapItem *item;
	assert(item = malloc(sizeof(HeapItem)));
	item->key = key;
	item->data = data;
	
	// insert new item and sift up
	h->items[h->next] = item;
	sift_up(h, h->next++);
}

int heap_remove_min(Heap *h)
{
	// check there is an item stored
	HeapItem *item = h->items[0];
	assert(item);
	// store the data and free the item memory
	int data = item->data;
	free(item);
	
	// swap in the last item then sift it down the heap
	h->items[0] = h->items[--h->next];
	h->items[h->next] = NULL;
	sift_down(h, 0);
	return data;
}

void heap_update_min(Heap *h, float key, int data)
{
	assert(!heap_empty(h));
	h->items[0]->key = key;
	h->items[0]->data = data;
	sift_down(h, 0);
}

int heap_peek_min(Heap *h)
{
	return h->items[0]->data;
}

float heap_peek_key(Heap *h)
{
	return h->items[0]->key;
}

void free_heap(Heap *h)
{
	assert(h);
	int i;
	for (i = 0; i < h->next; i++) free(h->items[i]);
	free(h->items);
	free(h);
}

void sift_up(Heap *h, int i)
{
	int p = (i - 1) / 2;
	if (i && h->items[i]->key < h->items[p]->key)
	{
		heap_swap_items(h, i, p);
		// check parent is not the root, then sift up again
		if (p) sift_up(h, p);
	}
}

void sift_down(Heap *h, int i)
{
	int c = min_child(h, i);
	if (c && h->items[i]->key > h->items[c]->key)
	{
		heap_swap_items(h, i, c);
		// check child is in the heap, then sift down again
		if (min_child(h, c)) sift_down(h, c);
	}
}

int min_child(Heap *h, int i)
{
	int c = 2 * i + 1;
	// children are out of the heap
	if (c >= h->next) return 0;
	// i has one child, or the first child is min
	if (c == h->next - 1 || 
		h->items[c]->key < h->items[c + 1]->key) return c;
	return c + 1;
}

void heap_swap_items(Heap *h, int a, int b)
{
	HeapItem *tmp = h->items[a];
	h->items[a] = h->items[b];
	h->items[b] = tmp;
}
