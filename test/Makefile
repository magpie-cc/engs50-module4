# Makefile for crawler
CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib -g
LIBS=-lutils -lcurl

test_pageio: test_pageio.c
	gcc $(CFLAGS) test_pageio.c $(LIBS) -o $@

clean:
	rm -f test_pageio
