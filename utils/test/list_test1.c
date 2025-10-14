/* 
 * list_test1.c --- 
 * 
 * Description: test put() to empty list and get() from non-empty list
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "car_helpers.h"

int main(void) {
	car_t *cp;
	int result;
	
	printf("=== Test 1: Put to Empty List & Get ===\n");
	
	/* Test 1: put() to empty list */
	printf("\nTest 1.1: Put to empty list\n");
	cp = make_car("ABC123", 15000.0, 2015);
	result = lput(cp);
	if (result == 0)
		printf("✓ Put to empty list succeeded\n");
	else
		printf("✗ Put to empty list failed\n");
	
	/* Test 2: get() from non-empty list */
	printf("\nTest 1.2: Get from non-empty list\n");
	cp = lget();
	if (cp != NULL) {
		printf("✓ Got car: ");
		print_car(cp);
		free(cp);
	} else {
		printf("✗ Get returned NULL\n");
	}
	
	/* Test 3: get() from empty list */
	printf("\nTest 1.3: Get from empty list\n");
	cp = lget();
	if (cp == NULL)
		printf("✓ Get from empty list correctly returned NULL\n");
	else
		printf("✗ Get from empty list should return NULL\n");
	
	exit(EXIT_SUCCESS);
}
