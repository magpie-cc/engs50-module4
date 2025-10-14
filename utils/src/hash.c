/* 
 * hash.c -- implements a generic hash table as an indexed set of queues.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"
#include "queue.h"

/* 
 * SuperFastHash() -- produces a number between 0 and the tablesize-1.
 * 
 * The following (rather complicated) code, has been taken from Paul
 * Hsieh's website under the terms of the BSD license. It's a hash
 * function used all over the place nowadays, including Google Sparse
 * Hash.
 */
#define get16bits(d) (*((const uint16_t *) (d)))

static uint32_t SuperFastHash (const char *data, int len, uint32_t tablesize) {
	uint32_t hash = len, tmp;
	int rem;
	
	if (len <= 0 || data == NULL)
		return 0;
	
	rem = len & 3;
	len >>= 2;
	
	/* Main loop */
	for (;len > 0; len--) {
		hash  += get16bits (data);
		tmp    = (get16bits (data+2) << 11) ^ hash;
		hash   = (hash << 16) ^ tmp;
		data  += 2*sizeof (uint16_t);
		hash  += hash >> 11;
	}
	
	/* Handle end cases */
	switch (rem) {
	case 3:
		hash += get16bits (data);
		hash ^= hash << 16;
		hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
		hash += hash >> 11;
		break;
	case 2:
		hash += get16bits (data);
		hash ^= hash << 11;
		hash += hash >> 17;
		break;
	case 1:
		hash += (signed char)*data;
		hash ^= hash << 10;
		hash += hash >> 1;
	}
	
	/* Force "avalanching" of final 127 bits */
	hash ^= hash << 3;
	hash += hash >> 5;
	hash ^= hash << 4;
	hash += hash >> 17;
	hash ^= hash << 25;
	hash += hash >> 6;
	
	return hash % tablesize;
}

/* Internal hash table structure */
typedef struct hashtable {
	uint32_t size;      /* number of slots in the table */
	queue_t **table;    /* array of queue pointers */
} hashtable_internal_t;

/* 
 * hopen -- opens a hash table with initial size hsize 
 */
hashtable_t *hopen(uint32_t hsize) {
	hashtable_internal_t *htp;
	uint32_t i;
	
	if (hsize == 0)
		return NULL;
	
	/* Allocate hash table structure */
	htp = (hashtable_internal_t*)malloc(sizeof(hashtable_internal_t));
	if (htp == NULL) {
		printf("[Error: malloc failed in hopen]\n");
		return NULL;
	}
	
	/* Allocate array of queue pointers */
	htp->table = (queue_t**)malloc(hsize * sizeof(queue_t*));
	if (htp->table == NULL) {
		printf("[Error: malloc failed for table array]\n");
		free(htp);
		return NULL;
	}
	
	htp->size = hsize;
	
	/* Create a queue for each slot */
	for (i = 0; i < hsize; i++) {
		htp->table[i] = qopen();
		if (htp->table[i] == NULL) {
			/* Cleanup on failure */
			for (uint32_t j = 0; j < i; j++)
				qclose(htp->table[j]);
			free(htp->table);
			free(htp);
			return NULL;
		}
	}
	
	return (hashtable_t*)htp;
}

/* 
 * hclose -- closes a hash table 
 */
void hclose(hashtable_t *htp) {
	hashtable_internal_t *ht;
	uint32_t i;
	
	if (htp == NULL)
		return;
	
	ht = (hashtable_internal_t*)htp;
	
	/* Close all queues */
	for (i = 0; i < ht->size; i++) {
		if (ht->table[i] != NULL)
			qclose(ht->table[i]);
	}
	
	/* Free table array and structure */
	free(ht->table);
	free(ht);
}

/* 
 * hput -- puts an entry into a hash table under designated key 
 * returns 0 for success; non-zero otherwise
 */
int32_t hput(hashtable_t *htp, void *ep, const char *key, int keylen) {
	hashtable_internal_t *ht;
	uint32_t index;
	
	if (htp == NULL || ep == NULL || key == NULL || keylen <= 0)
		return 1;
	
	ht = (hashtable_internal_t*)htp;
	
	/* Compute hash index */
	index = SuperFastHash(key, keylen, ht->size);
	
	/* Put element in the queue at this index */
	return qput(ht->table[index], ep);
}

/* 
 * happly -- applies a function to every entry in hash table 
 */
void happly(hashtable_t *htp, void (*fn)(void* ep)) {
	hashtable_internal_t *ht;
	uint32_t i;
	
	if (htp == NULL || fn == NULL)
		return;
	
	ht = (hashtable_internal_t*)htp;
	
	/* Apply function to every queue in the table */
	for (i = 0; i < ht->size; i++) {
		qapply(ht->table[i], fn);
	}
}

/* 
 * hsearch -- searches for an entry under a designated key using a
 * designated search fn -- returns a pointer to the entry or NULL if
 * not found
 */
void *hsearch(hashtable_t *htp, 
              bool (*searchfn)(void* elementp, const void* searchkeyp), 
              const char *key, 
              int32_t keylen) {
	hashtable_internal_t *ht;
	uint32_t index;
	
	if (htp == NULL || searchfn == NULL || key == NULL || keylen <= 0)
		return NULL;
	
	ht = (hashtable_internal_t*)htp;
	
	/* Compute hash index */
	index = SuperFastHash(key, keylen, ht->size);
	
	/* Search the queue at this index */
	return qsearch(ht->table[index], searchfn, key);
}

/* 
 * hremove -- removes and returns an entry under a designated key
 * using a designated search fn -- returns a pointer to the entry or
 * NULL if not found
 */
void *hremove(hashtable_t *htp, 
              bool (*searchfn)(void* elementp, const void* searchkeyp), 
              const char *key, 
              int32_t keylen) {
	hashtable_internal_t *ht;
	uint32_t index;
	
	if (htp == NULL || searchfn == NULL || key == NULL || keylen <= 0)
		return NULL;
	
	ht = (hashtable_internal_t*)htp;
	
	/* Compute hash index */
	index = SuperFastHash(key, keylen, ht->size);
	
	/* Remove from the queue at this index */
	return qremove(ht->table[index], searchfn, key);
}