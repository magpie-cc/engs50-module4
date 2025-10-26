# Makefile for crawler
CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

all: test_pageio test_indexio

test_pageio: test_pageio.c
	gcc $(CFLAGS) test_pageio.c $(LIBS) -o $@

test_indexio: test_indexio.c
	gcc $(CFLAGS) test_indexio.c $(LIBS) -o $@


clean:
	rm -f test_pageio
