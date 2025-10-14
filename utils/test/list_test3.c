/* 
 * list_test3.c --- 
 * 
 * Description: test apply() function
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "car_helpers.h"

int main(void) {
	printf("=== Test 3: Apply Function ===\n");
	
	/* Test apply on empty list */
	printf("\nTest 3.1: Apply to empty list\n");
	lapply(print_car);
	printf("✓ Apply to empty list completed (should print nothing)\n");
	
	/* Add some cars */
	printf("\nAdding three cars...\n");
	lput(make_car("XXX111", 5000.0, 2005));
	lput(make_car("YYY222", 15000.0, 2010));
	lput(make_car("ZZZ333", 25000.0, 2015));
	
	/* Test apply on non-empty list */
	printf("\nTest 3.2: Apply to non-empty list\n");
	printf("Cars in list:\n");
	lapply(print_car);
	
	/* Clean up */
	car_t *cp;
	while ((cp = lget()) != NULL)
		free(cp);
	
	exit(EXIT_SUCCESS);
}
