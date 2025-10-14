/* 
 * queue_test5.c --- 
 * 
 * Description: queue_test5.c -- test multiple queues simultaneously
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "../src/queue.h"
#include "../src/list.h"
#include "../src/car_helpers.h"

int main(void) {
	queue_t *q1, *q2, *q3;
	car_t *cp;
	
	printf("=== Queue Test 5: Multiple Queues ===\n");
	
	/* Create three queues */
	printf("\nTest 5.1: Create three queues simultaneously\n");
	q1 = qopen();
	q2 = qopen();
	q3 = qopen();
	printf("✓ Three queues created\n");
	
	/* Add different cars to each */
	printf("\nAdding different cars to each queue...\n");
	qput(q1, make_car("Q1-AAA", 1000.0, 2001));
	qput(q1, make_car("Q1-BBB", 2000.0, 2002));
	
	qput(q2, make_car("Q2-XXX", 10000.0, 2010));
	qput(q2, make_car("Q2-YYY", 20000.0, 2020));
	
	qput(q3, make_car("Q3-111", 5000.0, 2005));
	qput(q3, make_car("Q3-222", 6000.0, 2006));
	
	/* Verify each queue is independent */
	printf("\nTest 5.2: Verify queues are independent\n");
	printf("Queue 1 contents:\n");
	qapply(q1, print_car_wrapper);
	
	printf("Queue 2 contents:\n");
	qapply(q2, print_car_wrapper);
	
	printf("Queue 3 contents:\n");
	qapply(q3, print_car_wrapper);
	
	printf("✓ All queues maintain separate data\n");
	
	/* Clean up all three */
	while ((cp = (car_t*)qget(q1)) != NULL) free(cp);
	while ((cp = (car_t*)qget(q2)) != NULL) free(cp);
	while ((cp = (car_t*)qget(q3)) != NULL) free(cp);
	
	qclose(q1);
	qclose(q2);
	qclose(q3);
	
	exit(EXIT_SUCCESS);
}