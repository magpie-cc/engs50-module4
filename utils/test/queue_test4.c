/* 
 * queue_test4.c --- 
 * 
 * Description: queue_test4.c -- test qconcat
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/queue.h"
#include "../src/list.h"
#include "../src/car_helpers.h"

int main(void) {
	queue_t *q1, *q2;
	car_t *cp;
	
	printf("=== Queue Test 4: Concatenate ===\n");
	
	/* Create two queues */
	q1 = qopen();
	q2 = qopen();
	
	/* Add to q1 */
	printf("\nAdding to Queue 1: AAA111, BBB222\n");
	qput(q1, make_car("AAA111", 10000.0, 2010));
	qput(q1, make_car("BBB222", 20000.0, 2015));
	
	/* Add to q2 */
	printf("Adding to Queue 2: CCC333, DDD444\n");
	qput(q2, make_car("CCC333", 30000.0, 2020));
	qput(q2, make_car("DDD444", 40000.0, 2021));
	
	/* Concatenate */
	printf("\nTest 4.1: Concatenate q2 into q1\n");
	qconcat(q1, q2);
	printf("✓ Concatenation completed\n");
	
	/* q2 is now invalid/closed */
	
	printf("\nContents of q1 after concatenation (should be 4 cars in order):\n");
	qapply(q1, print_car_wrapper);
	
	/* Verify FIFO order */
	printf("\nTest 4.2: Verify FIFO order after concat\n");
	cp = (car_t*)qget(q1);
	if (cp && strcmp(cp->plate, "AAA111") == 0) {
		printf("✓ First: ");
		print_car(cp);
		free(cp);
	}
	
	cp = (car_t*)qget(q1);
	if (cp && strcmp(cp->plate, "BBB222") == 0) {
		printf("✓ Second: ");
		print_car(cp);
		free(cp);
	}
	
	cp = (car_t*)qget(q1);
	if (cp && strcmp(cp->plate, "CCC333") == 0) {
		printf("✓ Third: ");
		print_car(cp);
		free(cp);
	}
	
	cp = (car_t*)qget(q1);
	if (cp && strcmp(cp->plate, "DDD444") == 0) {
		printf("✓ Fourth: ");
		print_car(cp);
		free(cp);
	}
	
	qclose(q1);
	
	exit(EXIT_SUCCESS);
}
