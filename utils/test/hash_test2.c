/* 
 * hash_test2.c --- 
 * 
 * Description: hash_test2.c -- test happly function
 * 
 */

/* 
 * hash_test2.c -- test happly function
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../src/hash.h"
#include "../src/list.h"
#include "../src/car_helpers.h"

/* Search function - needed for cleanup */
bool search_car_by_plate(void *elementp, const void *keyp) {
	car_t *cp = (car_t*)elementp;
	const char *plate = (const char*)keyp;
	return (strcmp(cp->plate, plate) == 0);
}

int main(void) {
	hashtable_t *htp;
	car_t *cp;
	
	printf("=== Hash Test 2: Apply Function ===\n");
	
	htp = hopen(5);
	
	/* Test apply on empty hash table */
	printf("\nTest 2.1: Apply to empty hash table\n");
	happly(htp, print_car_wrapper);
	printf("✓ Apply completed (no output expected)\n");
	
	/* Add some cars */
	printf("\nTest 2.2: Add five cars\n");
	cp = make_car("XXX111", 5000.0, 2005);
	hput(htp, cp, cp->plate, strlen(cp->plate));
	
	cp = make_car("YYY222", 15000.0, 2010);
	hput(htp, cp, cp->plate, strlen(cp->plate));
	
	cp = make_car("ZZZ333", 25000.0, 2015);
	hput(htp, cp, cp->plate, strlen(cp->plate));
	
	cp = make_car("AAA444", 35000.0, 2020);
	hput(htp, cp, cp->plate, strlen(cp->plate));
	
	cp = make_car("BBB555", 45000.0, 2021);
	hput(htp, cp, cp->plate, strlen(cp->plate));
	
	printf("✓ Added 5 cars\n");
	
	/* Apply to non-empty hash table */
	printf("\nTest 2.3: Apply to non-empty hash table\n");
	printf("All cars in hash table:\n");
	happly(htp, print_car_wrapper);
	
	/* Clean up - remove and free all cars */
	cp = (car_t*)hremove(htp, search_car_by_plate, "XXX111", strlen("XXX111"));
	if (cp) free(cp);
	cp = (car_t*)hremove(htp, search_car_by_plate, "YYY222", strlen("YYY222"));
	if (cp) free(cp);
	cp = (car_t*)hremove(htp, search_car_by_plate, "ZZZ333", strlen("ZZZ333"));
	if (cp) free(cp);
	cp = (car_t*)hremove(htp, search_car_by_plate, "AAA444", strlen("AAA444"));
	if (cp) free(cp);
	cp = (car_t*)hremove(htp, search_car_by_plate, "BBB555", strlen("BBB555"));
	if (cp) free(cp);
	
	hclose(htp);
	exit(EXIT_SUCCESS);
}