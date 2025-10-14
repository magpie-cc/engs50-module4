/* 
 * car_helpers.c ---
 * 
 * Description: helper functions for testing
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

/* Helper function to create a car */
car_t *make_car(char *plate, double price, int year) {
	car_t *cp;
	
	cp = (car_t*)malloc(sizeof(car_t));
	if (cp == NULL) {
		printf("[Error: malloc failed]\n");
		return NULL;
	}
	
	strncpy(cp->plate, plate, MAXREG);
	cp->plate[MAXREG-1] = '\0';  /* ensure null termination */
	cp->price = price;
	cp->year = year;
	cp->next = NULL;
	
	return cp;
}

/* Helper function to print a car */
void print_car(car_t *cp) {
	if (cp != NULL)
		printf("[%s, $%.2f, %d]\n", cp->plate, cp->price, cp->year);
}

/* Wrapper for qapply - casts void* to car_t* */
void print_car_wrapper(void *elementp) {
	print_car((car_t*)elementp);
}