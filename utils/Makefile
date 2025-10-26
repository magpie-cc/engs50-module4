# Makefile for utils library
CFLAGS=-Wall -pedantic -std=c11 -I. -g

OFILES=queue.o hash.o webpage.o pageio.o indexio.o

all: $(OFILES)
	ar cr ../lib/libutils.a $(OFILES)

queue.o: queue.c queue.h
	gcc $(CFLAGS) -c queue.c

hash.o: hash.c hash.h queue.h
	gcc $(CFLAGS) -c hash.c

webpage.o: webpage.c webpage.h
	gcc $(CFLAGS) -c webpage.c

pageio.o: pageio.c pageio.h
	gcc $(CFLAGS) -c pageio.c

indexio.o: indexio.c indexio.h
	gcc $(CFLAGS) -c indexio.c

clean:
	rm -f *.o ../lib/libutils.a
