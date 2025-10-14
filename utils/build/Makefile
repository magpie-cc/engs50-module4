# Makefile for Module 3 - Queue and List
CFLAGS=-Wall -pedantic -std=c11 -I../src -g
OFILES=queue.o hash.o webpage.o
SRCDIR=../src
TESTDIR=../test
LIBDIR=../../../lib

# All test programs
all: list_test1 list_test2 list_test3 list_test4 \
     queue_test1 queue_test2 queue_test3 queue_test4 queue_test5 \
     hash_test1 hash_test2 hash_test3 $(OFILES)
	ar cr $(LIBDIR)/libutils.a $(OFILES)

# Source object files
list.o: $(SRCDIR)/list.c $(SRCDIR)/list.h
	gcc $(CFLAGS) -c $(SRCDIR)/list.c

queue.o: $(SRCDIR)/queue.c $(SRCDIR)/queue.h
	gcc $(CFLAGS) -c $(SRCDIR)/queue.c

car_helpers.o: $(SRCDIR)/car_helpers.c $(SRCDIR)/car_helpers.h $(SRCDIR)/list.h
	gcc $(CFLAGS) -c $(SRCDIR)/car_helpers.c

webpage.o: $(SRCDIR)/webpage.c $(SRCDIR)/webpage.h
	gcc $(CFLAGS) -c $(SRCDIR)/webpage.c -o webpage.o

# List test object files
list_test1.o: $(TESTDIR)/list_test1.c $(SRCDIR)/list.h $(SRCDIR)/car_helpers.h
	gcc $(CFLAGS) -c $(TESTDIR)/list_test1.c

list_test2.o: $(TESTDIR)/list_test2.c $(SRCDIR)/list.h $(SRCDIR)/car_helpers.h
	gcc $(CFLAGS) -c $(TESTDIR)/list_test2.c

list_test3.o: $(TESTDIR)/list_test3.c $(SRCDIR)/list.h $(SRCDIR)/car_helpers.h
	gcc $(CFLAGS) -c $(TESTDIR)/list_test3.c

list_test4.o: $(TESTDIR)/list_test4.c $(SRCDIR)/list.h $(SRCDIR)/car_helpers.h
	gcc $(CFLAGS) -c $(TESTDIR)/list_test4.c

# Queue test object files
queue_test1.o: $(TESTDIR)/queue_test1.c $(SRCDIR)/queue.h $(SRCDIR)/list.h $(SRCDIR)/car_helpers.h
	gcc $(CFLAGS) -c $(TESTDIR)/queue_test1.c

queue_test2.o: $(TESTDIR)/queue_test2.c $(SRCDIR)/queue.h $(SRCDIR)/list.h $(SRCDIR)/car_helpers.h
	gcc $(CFLAGS) -c $(TESTDIR)/queue_test2.c

queue_test3.o: $(TESTDIR)/queue_test3.c $(SRCDIR)/queue.h $(SRCDIR)/list.h $(SRCDIR)/car_helpers.h
	gcc $(CFLAGS) -c $(TESTDIR)/queue_test3.c

queue_test4.o: $(TESTDIR)/queue_test4.c $(SRCDIR)/queue.h $(SRCDIR)/list.h $(SRCDIR)/car_helpers.h
	gcc $(CFLAGS) -c $(TESTDIR)/queue_test4.c

queue_test5.o: $(TESTDIR)/queue_test5.c $(SRCDIR)/queue.h $(SRCDIR)/list.h $(SRCDIR)/car_helpers.h
	gcc $(CFLAGS) -c $(TESTDIR)/queue_test5.c

# List test executables
list_test1: list_test1.o list.o car_helpers.o
	gcc $(CFLAGS) list_test1.o list.o car_helpers.o -o list_test1

list_test2: list_test2.o list.o car_helpers.o
	gcc $(CFLAGS) list_test2.o list.o car_helpers.o -o list_test2

list_test3: list_test3.o list.o car_helpers.o
	gcc $(CFLAGS) list_test3.o list.o car_helpers.o -o list_test3

list_test4: list_test4.o list.o car_helpers.o
	gcc $(CFLAGS) list_test4.o list.o car_helpers.o -o list_test4

# Queue test executables
queue_test1: queue_test1.o queue.o car_helpers.o
	gcc $(CFLAGS) queue_test1.o queue.o car_helpers.o -o queue_test1

queue_test2: queue_test2.o queue.o car_helpers.o
	gcc $(CFLAGS) queue_test2.o queue.o car_helpers.o -o queue_test2

queue_test3: queue_test3.o queue.o car_helpers.o
	gcc $(CFLAGS) queue_test3.o queue.o car_helpers.o -o queue_test3

queue_test4: queue_test4.o queue.o car_helpers.o
	gcc $(CFLAGS) queue_test4.o queue.o car_helpers.o -o queue_test4

queue_test5: queue_test5.o queue.o car_helpers.o
	gcc $(CFLAGS) queue_test5.o queue.o car_helpers.o -o queue_test5

clean:
	rm -f *.o list_test* queue_test* hash_test*


# Hash test object files
hash_test1.o: $(TESTDIR)/hash_test1.c $(SRCDIR)/hash.h $(SRCDIR)/list.h $(SRCDIR)/car_helpers.h
	gcc $(CFLAGS) -c $(TESTDIR)/hash_test1.c

hash_test2.o: $(TESTDIR)/hash_test2.c $(SRCDIR)/hash.h $(SRCDIR)/list.h $(SRCDIR)/car_helpers.h
	gcc $(CFLAGS) -c $(TESTDIR)/hash_test2.c

hash_test3.o: $(TESTDIR)/hash_test3.c $(SRCDIR)/hash.h $(SRCDIR)/list.h $(SRCDIR)/car_helpers.h
	gcc $(CFLAGS) -c $(TESTDIR)/hash_test3.c

# Hash object file
hash.o: $(SRCDIR)/hash.c $(SRCDIR)/hash.h
	gcc $(CFLAGS) -c $(SRCDIR)/hash.c

# Hash test executables
hash_test1: hash_test1.o hash.o queue.o car_helpers.o
	gcc $(CFLAGS) hash_test1.o hash.o queue.o car_helpers.o -o hash_test1

hash_test2: hash_test2.o hash.o queue.o car_helpers.o
	gcc $(CFLAGS) hash_test2.o hash.o queue.o car_helpers.o -o hash_test2

hash_test3: hash_test3.o hash.o queue.o car_helpers.o
	gcc $(CFLAGS) hash_test3.o hash.o queue.o car_helpers.o -o hash_test3

test: all
	@echo "=== Running List Tests ==="
	./list_test1
	./list_test2
	./list_test3
	./list_test4
	@echo ""
	@echo "=== Running Queue Tests ==="
	./queue_test1
	./queue_test2
	./queue_test3
	./queue_test4
	./queue_test5
	@echo ""
	@echo "=== Running Hash Tests ==="
	./hash_test1
	./hash_test2
	./hash_test3
