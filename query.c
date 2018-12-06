/*
 * Implemenations of required functions for COMP20007 - Assignment 1.
 * See Assignment spec for more details
 *
 * Author: James Barnes (820946)
 */

#define PRINT_FORMAT "%6d %.6f\n"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "query.h"
#include "heap.h"


/* print the top results in the required format
 */
void print_results(Heap *top_results);

/* insert a result into the top_results heap if score is greater than the min
 */
void insert_result(Heap *top_results, int id, float score);

// see Assignment spec: 'Task 1: Array-based accumulation approach'
void print_array_results(Index *index, int n_results, int n_documents) 
{
	// check there is at least one result requested, and at least one docuement
	if (n_results <= 0 || n_documents <= 0) 
	{
		return;
	}
	
	int i;
	Node *node;
	Document *doc;
	Heap *top_results = new_heap(n_results);
	
	// build an array of total document scores, all 0.0
	float *scores;
	assert(scores = malloc(sizeof(float) * n_documents));
	for (i = 0; i < n_documents; i++) 
	{
		scores[i] = 0.0;
	}
	
	for (i = 0; i < index->num_terms; i++)
	{
		// traverse each doclist and add to each corresponding score
		for (node = index->doclists[i]->head; node; node = node->next)
		{
			doc = node->data;
			scores[doc->id] += doc->score;
		}
	}
	
	// iterate over scores, using a heap to keep the top scores
	for (i = 0; i < n_documents; i++)
	{
		insert_result(top_results, i, scores[i]);
	}
	
	print_results(top_results);
	
	// clean up
	free_heap(top_results);
	free(scores);
}

// see Assignment spec: 'Task 2: Priority queue-based multi-way merge approach'
void print_merge_results(Index *index, int n_results) 
{
	// check there is at least one result requested
	if (n_results <= 0) 
	{
		return;
	}
	
	int i, num_terms = index->num_terms, id = -1;
	float score = 0.0;
	Heap *doc_heap = new_heap(num_terms), *top_results = new_heap(n_results);
	Node **doc_nodes, *node;
	assert(doc_nodes = malloc(sizeof(Node*) * num_terms));
	Document *doc;
	
	// build a heap of doclists (key: next id, val: next node in list)
	for (i = 0; i < num_terms; i++)
	{
		if ((doc_nodes[i] = index->doclists[i]->head)) 
		{
			doc = doc_nodes[i]->data;
			heap_insert(doc_heap, doc->id, i);
		}
	}
	
	int count = 0;
	while (!heap_empty(doc_heap))
	{
		// get the next document from the lowest in doc_nodes by id
		i = heap_peek_min(doc_heap);
		node = doc_nodes[i];
		doc = node->data;
		// add to score if ids match
		if (doc->id == id) 
		{
			score += doc->score;
		}
		else
		{
			// add to results (if required)
			count++;
			insert_result(top_results, id, score);
			// start tracking the next doc
			id = doc->id;
			score = doc->score;
		}
		
		// update the doclist in the doc heap if doclist is non-empty
		if ((doc_nodes[i] = node->next))
		{
			doc = doc_nodes[i]->data;
			heap_update_min(doc_heap, doc->id, i);
		}
		// else, remove it, we're done with that doclist
		else 
		{
			heap_remove_min(doc_heap);
		}
	}
	// add last doc to results (if required)
	insert_result(top_results, id, score);
	
	print_results(top_results);
	
	// clean up
	free_heap(top_results);
	free_heap(doc_heap);
	free(doc_nodes);
}

void print_results(Heap *top_results)
{
	int count = heap_count(top_results), i;
	Document **top_docs;
	assert(top_docs = malloc(sizeof(Document*) * count));
	Document *doc;
	int id;
	float score;
	// build an array of docs in decending score order
	for (i = count - 1; i >= 0; i--)
	{
		score = heap_peek_key(top_results);
		id = heap_remove_min(top_results);
		top_docs[i] = new_document(id, score);
	}
	// print the formatted score for each document in descending order
	for (i = 0; i < count; i++)
	{
		doc = top_docs[i];
		printf(PRINT_FORMAT, doc->id, doc->score);
		free_document(doc);
	}
	free(top_docs);
}

void insert_result(Heap *top_results, int id, float score)
{
	// check for non-zero score
	if (score)
	{
		// update top_results to contain the new result if score is greater
		// than the current min
		if (heap_full(top_results) && score > heap_peek_key(top_results))
		{
			heap_update_min(top_results, score, id);
		}
		// else, insert if heap is not full
		else if (!heap_full(top_results)) 
		{
			heap_insert(top_results, score, id);
		}
	}
}
