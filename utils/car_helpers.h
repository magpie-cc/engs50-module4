#pragma once
#include "list.h"

car_t *make_car(char *plate, double price, int year);
void print_car(car_t *cp);

#pragma once
#include "list.h"

car_t *make_car(char *plate, double price, int year);
void print_car(car_t *cp);
void print_car_wrapper(void *elementp);  /* Add this line */