/* 
 * list.c -- implementation of a list of cars
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

/* Hidden global list - only accessible within this module */
static car_t *front = NULL;

/* 
 * lput() - place a car at the beginning of the list
 * returns 0 if successful; nonzero otherwise 
 */
int32_t lput(car_t *cp) {
	if (cp == NULL) {
		printf("[Error: cannot put NULL car]\n");
		return 1;
	}
	
	cp->next = front;    /* new car points to current front */
	front = cp;          /* new car becomes the front */
	return 0;
}

/* 
 * lget() - remove and return the first car in the list
 * return NULL if the list is empty
 */
car_t *lget() {
	car_t *result;
	
	if (front == NULL)
		return NULL;
	
	result = front;          /* save pointer to first car */
	front = front->next;     /* move front to next car */
	result->next = NULL;     /* disconnect returned car from list */
	return result;
}

/* 
 * lapply() - apply a function to every car in the list 
 */
void lapply(void (*fn)(car_t *cp)) {
	car_t *p;
	
	if (fn == NULL)
		return;
	
	for (p = front; p != NULL; p = p->next)
		fn(p);
}

/* 
 * lremove() - find, remove, and return any car with the designated plate
 * return NULL if not present
 */
car_t *lremove(char *platep) {
	car_t *p, *prev;
	
	if (platep == NULL || front == NULL)
		return NULL;
	
	/* Check if it's the first car */
	if (strcmp(front->plate, platep) == 0) {
		p = front;
		front = front->next;
		p->next = NULL;
		return p;
	}
	
	/* Search through the rest of the list */
	prev = front;
	for (p = front->next; p != NULL; prev = p, p = p->next) {
		if (strcmp(p->plate, platep) == 0) {
			prev->next = p->next;  /* bypass this car */
			p->next = NULL;        /* disconnect from list */
			return p;
		}
	}
	
	return NULL;  /* not found */
}