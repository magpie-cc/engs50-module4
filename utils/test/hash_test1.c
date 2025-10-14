/* 
 * hash_test1.c --- 
 * 
 * Description: hash_test1.c -- test basic hash table operations
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../src/hash.h"
#include "../src/list.h"
#include "../src/car_helpers.h"

/* Search function for cars by plate */
bool search_car_by_plate(void *elementp, const void *keyp) {
	car_t *cp = (car_t*)elementp;
	const char *plate = (const char*)keyp;
	return (strcmp(cp->plate, plate) == 0);
}

int main(void) {
	hashtable_t *htp;
	car_t *cp;
	
	printf("=== Hash Test 1: Basic Operations ===\n");
	
	/* Test 1: Open hash table */
	printf("\nTest 1.1: Open hash table with size 10\n");
	htp = hopen(10);
	if (htp != NULL)
		printf("✓ Hash table opened successfully\n");
	else {
		printf("✗ Failed to open hash table\n");
		exit(EXIT_FAILURE);
	}
	
	/* Test 2: Put elements */
	printf("\nTest 1.2: Put three cars into hash table\n");
	cp = make_car("AAA111", 10000.0, 2010);
	if (hput(htp, cp, cp->plate, strlen(cp->plate)) == 0)
		printf("✓ Put AAA111 succeeded\n");
	
	cp = make_car("BBB222", 20000.0, 2015);
	if (hput(htp, cp, cp->plate, strlen(cp->plate)) == 0)
		printf("✓ Put BBB222 succeeded\n");
	
	cp = make_car("CCC333", 30000.0, 2020);
	if (hput(htp, cp, cp->plate, strlen(cp->plate)) == 0)
		printf("✓ Put CCC333 succeeded\n");
	
	/* Test 3: Search for existing element */
	printf("\nTest 1.3: Search for BBB222\n");
	cp = (car_t*)hsearch(htp, search_car_by_plate, "BBB222", strlen("BBB222"));
	if (cp != NULL && strcmp(cp->plate, "BBB222") == 0) {
		printf("✓ Found: ");
		print_car(cp);
	} else {
		printf("✗ Search failed\n");
	}
	
	/* Test 4: Search for non-existing element */
	printf("\nTest 1.4: Search for nonexistent ZZZ999\n");
	cp = (car_t*)hsearch(htp, search_car_by_plate, "ZZZ999", strlen("ZZZ999"));
	if (cp == NULL)
		printf("✓ Correctly returned NULL for nonexistent car\n");
	else
		printf("✗ Should return NULL\n");
	
	/* Test 5: Remove element */
	printf("\nTest 1.5: Remove BBB222\n");
	cp = (car_t*)hremove(htp, search_car_by_plate, "BBB222", strlen("BBB222"));
	if (cp != NULL && strcmp(cp->plate, "BBB222") == 0) {
		printf("✓ Removed: ");
		print_car(cp);
		free(cp);
	} else {
		printf("✗ Remove failed\n");
	}
	
	/* Test 6: Search for removed element */
	printf("\nTest 1.6: Search for removed BBB222\n");
	cp = (car_t*)hsearch(htp, search_car_by_plate, "BBB222", strlen("BBB222"));
	if (cp == NULL)
		printf("✓ Correctly returned NULL for removed car\n");
	else
		printf("✗ Should return NULL after removal\n");
	
	/* Clean up remaining elements */
	cp = (car_t*)hremove(htp, search_car_by_plate, "AAA111", strlen("AAA111"));
	if (cp) free(cp);
	cp = (car_t*)hremove(htp, search_car_by_plate, "CCC333", strlen("CCC333"));
	if (cp) free(cp);
	
	/* Test 7: Close hash table */
	printf("\nTest 1.7: Close hash table\n");
	hclose(htp);
	printf("✓ Hash table closed\n");
	
	exit(EXIT_SUCCESS);
}
