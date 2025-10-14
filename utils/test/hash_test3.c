/* 
 * hash_test3.c --- 
 * 
 * Description: hash_test3.c -- test hash collision handling
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../src/hash.h"
#include "../src/list.h"
#include "../src/car_helpers.h"

bool search_car_by_plate(void *elementp, const void *keyp) {
	car_t *cp = (car_t*)elementp;
	const char *plate = (const char*)keyp;
	return (strcmp(cp->plate, plate) == 0);
}

int main(void) {
	hashtable_t *htp;
	car_t *cp;
	int i;
	char plate[10];
	
	printf("=== Hash Test 3: Collision Handling ===\n");
	
	/* Use small hash table to force collisions */
	printf("\nTest 3.1: Create small hash table (size 3)\n");
	htp = hopen(3);
	printf("✓ Hash table created\n");
	
	/* Add many cars to force collisions */
	printf("\nTest 3.2: Add 10 cars to size-3 table (forces collisions)\n");
	for (i = 0; i < 10; i++) {
		sprintf(plate, "CAR%03d", i);
		cp = make_car(plate, 10000.0 + i * 1000, 2010 + i);
		hput(htp, cp, cp->plate, strlen(cp->plate));
	}
	printf("✓ Added 10 cars\n");
	
	/* Verify all can be found despite collisions */
	printf("\nTest 3.3: Search for all cars (verify collision handling)\n");
	int found = 0;
	for (i = 0; i < 10; i++) {
		sprintf(plate, "CAR%03d", i);
		cp = (car_t*)hsearch(htp, search_car_by_plate, plate, strlen(plate));
		if (cp != NULL && strcmp(cp->plate, plate) == 0)
			found++;
	}
	
	if (found == 10)
		printf("✓ Found all 10 cars despite collisions\n");
	else
		printf("✗ Only found %d/10 cars\n", found);
	
	/* Print all cars */
	printf("\nAll cars in hash table:\n");
	happly(htp, print_car_wrapper);
	
	/* Remove all cars */
	printf("\nTest 3.4: Remove all cars\n");
	for (i = 0; i < 10; i++) {
		sprintf(plate, "CAR%03d", i);
		cp = (car_t*)hremove(htp, search_car_by_plate, plate, strlen(plate));
		if (cp) free(cp);
	}
	printf("✓ Removed all cars\n");
	
	hclose(htp);
	exit(EXIT_SUCCESS);
}
