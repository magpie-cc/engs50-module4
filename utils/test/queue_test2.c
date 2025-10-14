/* 
 * queue_test2.c --- 
 * 
 * Description: queue_test2.c -- test qapply function
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include "../src/queue.h"
#include "../src/list.h"
#include "../src/car_helpers.h"

int main(void) {
	queue_t *qp;
	car_t *cp;
	
	printf("=== Queue Test 2: Apply Function ===\n");
	
	qp = qopen();
	
	/* Test on empty queue */
	printf("\nTest 2.1: Apply to empty queue\n");
	qapply(qp, print_car_wrapper);
	printf("✓ Apply to empty queue completed (no output expected)\n");
	
	/* Add some cars */
	printf("\nAdding three cars...\n");
	qput(qp, make_car("XXX111", 5000.0, 2005));
	qput(qp, make_car("YYY222", 15000.0, 2010));
	qput(qp, make_car("ZZZ333", 25000.0, 2015));
	
	/* Test apply on non-empty queue */
	printf("\nTest 2.2: Apply to non-empty queue\n");
	printf("Cars in queue (FIFO order):\n");
	qapply(qp, print_car_wrapper);
	
	/* Clean up */
	while ((cp = (car_t*)qget(qp)) != NULL)
		free(cp);
	qclose(qp);
	
	exit(EXIT_SUCCESS);
}