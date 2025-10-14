/* 
 * queue.c --- 
 * 
 * Description: queue.c -- implementation of a generic FIFO queue
 *
 * The queue is implemented as a linked list with front and back pointers
 * for O(1) put and get operations.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"

/* Internal node structure - hidden from users */
typedef struct qnode {
	void *element;           /* pointer to element stored in this node */
	struct qnode *next;      /* pointer to next node */
} qnode_t;

/* Internal queue structure - hidden from users */
typedef struct queue {
	qnode_t *front;          /* pointer to first node (for get) */
	qnode_t *back;           /* pointer to last node (for put) */
} queue_internal_t;

/* 
 * qopen -- create an empty queue 
 */
queue_t* qopen(void) {
	queue_internal_t *qp;
	
	qp = (queue_internal_t*)malloc(sizeof(queue_internal_t));
	if (qp == NULL) {
		printf("[Error: malloc failed in qopen]\n");
		return NULL;
	}
	
	qp->front = NULL;
	qp->back = NULL;
	
	return (queue_t*)qp;
}

/* 
 * qclose -- deallocate a queue, frees everything in it 
 */
void qclose(queue_t *qp) {
	queue_internal_t *q;
	qnode_t *curr, *next;
	
	if (qp == NULL)
		return;
	
	q = (queue_internal_t*)qp;
	
	/* Free all nodes (but not the elements - caller's responsibility) */
	curr = q->front;
	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}
	
	/* Free the queue structure itself */
	free(q);
}

/* 
 * qput -- put element at the end of the queue
 * returns 0 if successful; nonzero otherwise 
 */
int32_t qput(queue_t *qp, void *elementp) {
	queue_internal_t *q;
	qnode_t *newnode;
	
	if (qp == NULL || elementp == NULL)
		return 1;
	
	q = (queue_internal_t*)qp;
	
	/* Allocate new node */
	newnode = (qnode_t*)malloc(sizeof(qnode_t));
	if (newnode == NULL) {
		printf("[Error: malloc failed in qput]\n");
		return 1;
	}
	
	newnode->element = elementp;
	newnode->next = NULL;
	
	/* Add to back of queue */
	if (q->back == NULL) {
		/* Queue is empty */
		q->front = newnode;
		q->back = newnode;
	} else {
		/* Queue has elements */
		q->back->next = newnode;
		q->back = newnode;
	}
	
	return 0;
}

/* 
 * qget -- get the first element from queue, removing it from the queue 
 */
void* qget(queue_t *qp) {
	queue_internal_t *q;
	qnode_t *node;
	void *element;
	
	if (qp == NULL)
		return NULL;
	
	q = (queue_internal_t*)qp;
	
	if (q->front == NULL)
		return NULL;  /* Queue is empty */
	
	/* Remove first node */
	node = q->front;
	element = node->element;
	
	q->front = node->next;
	if (q->front == NULL)
		q->back = NULL;  /* Queue is now empty */
	
	free(node);
	return element;
}

/* 
 * qapply -- apply a function to every element of the queue 
 */
void qapply(queue_t *qp, void (*fn)(void* elementp)) {
	queue_internal_t *q;
	qnode_t *curr;
	
	if (qp == NULL || fn == NULL)
		return;
	
	q = (queue_internal_t*)qp;
	
	for (curr = q->front; curr != NULL; curr = curr->next)
		fn(curr->element);
}

/* 
 * qsearch -- search a queue using a supplied boolean function
 * returns a pointer to an element, or NULL if not found
 */
void* qsearch(queue_t *qp, 
              bool (*searchfn)(void* elementp, const void* keyp),
              const void* skeyp) {
	queue_internal_t *q;
	qnode_t *curr;
	
	if (qp == NULL || searchfn == NULL)
		return NULL;
	
	q = (queue_internal_t*)qp;
	
	for (curr = q->front; curr != NULL; curr = curr->next) {
		if (searchfn(curr->element, skeyp))
			return curr->element;
	}
	
	return NULL;  /* Not found */
}

/* 
 * qremove -- search a queue using a supplied boolean function,
 * removes the element from the queue and returns a pointer to it or
 * NULL if not found
 */
void* qremove(queue_t *qp,
              bool (*searchfn)(void* elementp, const void* keyp),
              const void* skeyp) {
	queue_internal_t *q;
	qnode_t *curr, *prev;
	void *element;
	
	if (qp == NULL || searchfn == NULL)
		return NULL;
	
	q = (queue_internal_t*)qp;
	prev = NULL;
	
	for (curr = q->front; curr != NULL; prev = curr, curr = curr->next) {
		if (searchfn(curr->element, skeyp)) {
			/* Found it - remove from queue */
			
			if (prev == NULL) {
				/* Removing first node */
				q->front = curr->next;
				if (q->front == NULL)
					q->back = NULL;  /* Queue is now empty */
			} else {
				/* Removing middle or last node */
				prev->next = curr->next;
				if (curr == q->back)
					q->back = prev;  /* Was last node */
			}
			
			element = curr->element;
			free(curr);
			return element;
		}
	}
	
	return NULL;  /* Not found */
}

/* 
 * qconcat -- concatenate elements of q2 into q1
 * q2 is deallocated, closed, and unusable upon completion 
 */
void qconcat(queue_t *q1p, queue_t *q2p) {
	queue_internal_t *q1, *q2;
	
	if (q1p == NULL || q2p == NULL)
		return;
	
	q1 = (queue_internal_t*)q1p;
	q2 = (queue_internal_t*)q2p;
	
	if (q2->front == NULL) {
		/* q2 is empty - just free it */
		free(q2);
		return;
	}
	
	if (q1->front == NULL) {
		/* q1 is empty - just move q2's nodes to q1 */
		q1->front = q2->front;
		q1->back = q2->back;
	} else {
		/* Both have elements - link them */
		q1->back->next = q2->front;
		q1->back = q2->back;
	}
	
	/* Free q2 structure (but not its nodes - now part of q1) */
	free(q2);
}
