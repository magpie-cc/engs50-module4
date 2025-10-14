/* 
 * list_test4.c --- 
 * 
 * Description: test remove() function
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "car_helpers.h"

int main(void) {
	car_t *cp;
	
	printf("=== Test 4: Remove Function ===\n");
	
	/* Test 4.1: Remove from empty list */
	printf("\nTest 4.1: Remove from empty list\n");
	cp = lremove("NOTHERE");
	if (cp == NULL)
		printf("✓ Remove from empty list returned NULL\n");
	else
		printf("✗ Remove from empty list should return NULL\n");
	
	/* Add three cars */
	printf("\nAdding three cars: AAA111, BBB222, CCC333\n");
	lput(make_car("AAA111", 10000.0, 2010));
	lput(make_car("BBB222", 20000.0, 2015));
	lput(make_car("CCC333", 30000.0, 2020));
	
	/* Test 4.2: Remove from beginning */
	printf("\nTest 4.2: Remove from beginning (CCC333)\n");
	cp = lremove("CCC333");
	if (cp != NULL && strcmp(cp->plate, "CCC333") == 0) {
		printf("✓ Removed: ");
		print_car(cp);
		free(cp);
	} else {
		printf("✗ Failed to remove from beginning\n");
	}
	
	printf("Remaining cars:\n");
	lapply(print_car);
	
	/* Test 4.3: Remove from end */
	printf("\nTest 4.3: Remove from end (AAA111)\n");
	cp = lremove("AAA111");
	if (cp != NULL && strcmp(cp->plate, "AAA111") == 0) {
		printf("✓ Removed: ");
		print_car(cp);
		free(cp);
	} else {
		printf("✗ Failed to remove from end\n");
	}
	
	printf("Remaining cars:\n");
	lapply(print_car);
	
	/* Add more cars for middle test */
	lput(make_car("DDD444", 40000.0, 2021));
	lput(make_car("EEE555", 50000.0, 2022));
	
	/* Test 4.4: Remove from middle */
	printf("\nTest 4.4: Remove from middle (BBB222)\n");
	printf("Before removal:\n");
	lapply(print_car);
	
	cp = lremove("BBB222");
	if (cp != NULL && strcmp(cp->plate, "BBB222") == 0) {
		printf("✓ Removed: ");
		print_car(cp);
		free(cp);
	} else {
		printf("✗ Failed to remove from middle\n");
	}
	
	printf("After removal:\n");
	lapply(print_car);
	
	/* Clean up */
	while ((cp = lget()) != NULL)
		free(cp);
	
	exit(EXIT_SUCCESS);
}
