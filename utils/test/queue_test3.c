/* 
 * queue_test3.c --- 
 * 
 * Description: queue_test3.c -- test qsearch and qremove
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../src/queue.h"
#include "../src/list.h"
#include "../src/car_helpers.h"

/* Search function - matches plate */
bool search_by_plate(void *elementp, const void *keyp) {
	car_t *cp = (car_t*)elementp;
	char *plate = (char*)keyp;
	return (strcmp(cp->plate, plate) == 0);
}

int main(void) {
	queue_t *qp;
	car_t *cp;
	
	printf("=== Queue Test 3: Search and Remove ===\n");
	
	qp = qopen();
	
	/* Add test data */
	printf("\nAdding three cars: AAA111, BBB222, CCC333\n");
	qput(qp, make_car("AAA111", 10000.0, 2010));
	qput(qp, make_car("BBB222", 20000.0, 2015));
	qput(qp, make_car("CCC333", 30000.0, 2020));
	
	/* Test search - find existing */
	printf("\nTest 3.1: Search for BBB222\n");
	cp = (car_t*)qsearch(qp, search_by_plate, "BBB222");
	if (cp != NULL && strcmp(cp->plate, "BBB222") == 0) {
		printf("✓ Found: ");
		print_car(cp);
	} else {
		printf("✗ Search failed\n");
	}
	
	/* Test search - not found */
	printf("\nTest 3.2: Search for nonexistent DDD444\n");
	cp = (car_t*)qsearch(qp, search_by_plate, "DDD444");
	if (cp == NULL)
		printf("✓ Correctly returned NULL for nonexistent car\n");
	else
		printf("✗ Should return NULL for nonexistent car\n");
	
	/* Test remove - from middle */
	printf("\nTest 3.3: Remove BBB222 from middle\n");
	cp = (car_t*)qremove(qp, search_by_plate, "BBB222");
	if (cp != NULL && strcmp(cp->plate, "BBB222") == 0) {
		printf("✓ Removed: ");
		print_car(cp);
		free(cp);
	} else {
		printf("✗ Remove failed\n");
	}
	
	printf("Remaining cars:\n");
	qapply(qp, print_car_wrapper);
	
	/* Test remove - from front */
	printf("\nTest 3.4: Remove AAA111 from front\n");
	cp = (car_t*)qremove(qp, search_by_plate, "AAA111");
	if (cp != NULL && strcmp(cp->plate, "AAA111") == 0) {
		printf("✓ Removed: ");
		print_car(cp);
		free(cp);
	} else {
		printf("✗ Remove failed\n");
	}
	
	printf("Remaining cars:\n");
	qapply(qp, print_car_wrapper);
	
	/* Clean up */
	while ((cp = (car_t*)qget(qp)) != NULL)
		free(cp);
	qclose(qp);
	
	exit(EXIT_SUCCESS);
}