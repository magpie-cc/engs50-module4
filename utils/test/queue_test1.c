/* 
 * queue_test1.c --- 
 * 
 * Description: queue_test1.c -- test basic queue operations
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/queue.h"
#include "../src/list.h"  /* For car_t structure */
#include "../src/car_helpers.h"

int main(void) {
	queue_t *qp;
	car_t *cp;
	
	printf("=== Queue Test 1: Basic Operations ===\n");
	
	/* Test 1: Open a queue */
	printf("\nTest 1.1: Open queue\n");
	qp = qopen();
	if (qp != NULL)
		printf("✓ Queue opened successfully\n");
	else {
		printf("✗ Failed to open queue\n");
		exit(EXIT_FAILURE);
	}
	
	/* Test 2: Put to empty queue */
	printf("\nTest 1.2: Put to empty queue\n");
	cp = make_car("AAA111", 10000.0, 2010);
	if (qput(qp, cp) == 0)
		printf("✓ Put to empty queue succeeded\n");
	else
		printf("✗ Put failed\n");
	
	/* Test 3: Put to non-empty queue */
	printf("\nTest 1.3: Put to non-empty queue\n");
	cp = make_car("BBB222", 20000.0, 2015);
	if (qput(qp, cp) == 0)
		printf("✓ Put to non-empty queue succeeded\n");
	else
		printf("✗ Put failed\n");
	
	/* Test 4: Get from queue (FIFO - should get AAA111) */
	printf("\nTest 1.4: Get from queue (expect AAA111)\n");
	cp = (car_t*)qget(qp);
	if (cp != NULL && strcmp(cp->plate, "AAA111") == 0) {
		printf("✓ Got correct car (FIFO): ");
		print_car(cp);
		free(cp);
	} else {
		printf("✗ Get failed or wrong car\n");
	}
	
	/* Test 5: Get again (should get BBB222) */
	printf("\nTest 1.5: Get again (expect BBB222)\n");
	cp = (car_t*)qget(qp);
	if (cp != NULL && strcmp(cp->plate, "BBB222") == 0) {
		printf("✓ Got correct car: ");
		print_car(cp);
		free(cp);
	} else {
		printf("✗ Get failed or wrong car\n");
	}
	
	/* Test 6: Get from empty queue */
	printf("\nTest 1.6: Get from empty queue\n");
	cp = (car_t*)qget(qp);
	if (cp == NULL)
		printf("✓ Get from empty queue correctly returned NULL\n");
	else
		printf("✗ Get from empty queue should return NULL\n");
	
	/* Test 7: Close queue */
	printf("\nTest 1.7: Close queue\n");
	qclose(qp);
	printf("✓ Queue closed\n");
	
	exit(EXIT_SUCCESS);
}
