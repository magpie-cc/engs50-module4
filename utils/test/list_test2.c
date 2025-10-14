/* 
 * list_test2.c --- 
 * 
 * Description: test put() to non-empty list and multiple operations
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "car_helpers.h"

int main(void) {
	car_t *cp;
	
	printf("=== Test 2: Multiple Cars ===\n");
	
	/* Add three cars */
	printf("\nAdding three cars...\n");
	lput(make_car("AAA111", 10000.0, 2010));
	lput(make_car("BBB222", 20000.0, 2015));
	lput(make_car("CCC333", 30000.0, 2020));
	printf("✓ Added 3 cars\n");
	
	/* Get them back (should be in reverse order - stack behavior) */
	printf("\nGetting cars back:\n");
	cp = lget();
	if (cp != NULL) {
		print_car(cp);
		free(cp);
	}
	
	cp = lget();
	if (cp != NULL) {
		print_car(cp);
		free(cp);
	}
	
	cp = lget();
	if (cp != NULL) {
		print_car(cp);
		free(cp);
	}
	
	exit(EXIT_SUCCESS);
}
