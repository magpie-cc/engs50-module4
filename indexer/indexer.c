/* 
 * indexer.c - Step 3: Hash table of words
 * 
 * Description: Index webpage 1 into hash table with word counts
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <pageio.h>
#include <webpage.h>
#include <hash.h>

// Structure to hold a word and its count
typedef struct wordcount {
	char *word;
	int count;
} wordcount_t;

// Function prototypes
int NormalizeWord(char *w);
bool searchfn(void *elementp, const void *keyp);
void sumwords_fn(void *elementp);
void free_wordcount(void *elementp);

// Global variable for sum
int total_words = 0;

// Free a wordcount entry
void free_wordcount(void *elementp) {
	wordcount_t *wc = (wordcount_t*)elementp;
	if (wc != NULL) {
		free(wc->word);
		free(wc);
	}
}

int main(void) {
	int pos = 0;
	char *word;
	int id = 1;
	char *pagedir = "../pages";
	const uint32_t HASHTABLE_SIZE = 100;
	
	printf("Step 3: Hash Table of Words\n");
	printf("============================\n\n");
	
	// Create hash table
	hashtable_t *index = hopen(HASHTABLE_SIZE);
	if (index == NULL) {
		fprintf(stderr, "Error: failed to create hash table\n");
		return EXIT_FAILURE;
	}
	
	// Load page
	webpage_t *webpage = pageload(id, pagedir);
	if (webpage == NULL) {
		fprintf(stderr, "Error: failed to load page %d\n", id);
		hclose(index);
		return EXIT_FAILURE;
	}
	
	printf("Loaded page %d: %s\n\n", id, webpage_getURL(webpage));
	
	// Extract words and build index
	while ((pos = webpage_getNextWord(webpage, pos, &word)) > 0) {
		// Normalize word
		if (NormalizeWord(word) == 0) {
			// Search for word in hash table
			wordcount_t *wc = (wordcount_t*)hsearch(index, searchfn, word, strlen(word));
			
			if (wc != NULL) {
				// Word found - increment count
				wc->count++;
			} else {
				// Word not found - create new entry
				wc = (wordcount_t*)malloc(sizeof(wordcount_t));
				if (wc != NULL) {
					wc->word = strdup(word);
					wc->count = 1;
					hput(index, wc, word, strlen(word));
				}
			}
		}
		free(word);
	}
	
	// Sum all word counts
	total_words = 0;
	happly(index, sumwords_fn);
	
	printf("Total word count: %d\n", total_words);
	printf("Expected: 141\n");
	
	if (total_words == 141) {
		printf("✓ PASS: Count matches!\n");
	} else {
		printf("✗ FAIL: Count mismatch!\n");
	}
	
	// Cleanup
	happly(index, free_wordcount);
	hclose(index);
	webpage_delete(webpage);
	
	return EXIT_SUCCESS;
}

// Normalize word function
int NormalizeWord(char *wp) {
	if (wp == NULL || strlen(wp) < 3) return 1;
	
	char *p = wp;
	while (*p != '\0') {
		if (!isalpha((unsigned char)*p)) {
			return 1;
		}
		*p = tolower((unsigned char)*p);
		p++;
	}
	
	return 0;
}

// Search function for hash table
bool searchfn(void *elementp, const void *keyp) {
	wordcount_t *wc = (wordcount_t*)elementp;
	char *word = (char*)keyp;
	return strcmp(wc->word, word) == 0;
}

// Sum function to count all words
void sumwords_fn(void *elementp) {
	wordcount_t *wc = (wordcount_t*)elementp;
	total_words += wc->count;
}