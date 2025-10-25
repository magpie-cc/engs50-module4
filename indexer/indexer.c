/* 
 * indexer.c - Step 4: Queue of documents
 * 
 * Description: Index webpage with queue of documents per word
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
#include <queue.h>

// Structure to hold document ID and count
typedef struct doccount {
	int docID;
	int count;
} doccount_t;

// Structure to hold a word and its queue of documents
typedef struct wordnode {
	char *word;
	queue_t *docs;  // queue of doccount_t
} wordnode_t;

// Function prototypes
int NormalizeWord(char *w);
bool searchfn_word(void *elementp, const void *keyp);
bool searchfn_doc(void *elementp, const void *keyp);
void sumwords_fn(void *elementp);
void free_wordnode(void *elementp);

// Global variable for sum
int total_words = 0;

int main(void) {
	int pos = 0;
	char *word;
	int id = 1;
	char *pagedir = "../pages";
	const uint32_t HASHTABLE_SIZE = 100;
	
	printf("Step 4: Queue of Documents\n");
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
			wordnode_t *wn = (wordnode_t*)hsearch(index, searchfn_word, word, strlen(word));
			
			if (wn != NULL) {
				// Word found - search for document in queue
				doccount_t *dc = (doccount_t*)qsearch(wn->docs, searchfn_doc, &id);
				
				if (dc != NULL) {
					// Document found - increment count
					dc->count++;
				} else {
					// Document not found - create new entry
					dc = (doccount_t*)malloc(sizeof(doccount_t));
					if (dc != NULL) {
						dc->docID = id;
						dc->count = 1;
						qput(wn->docs, dc);
					}
				}
			} else {
				// Word not found - create new wordnode with queue
				wn = (wordnode_t*)malloc(sizeof(wordnode_t));
				if (wn != NULL) {
					wn->word = strdup(word);
					wn->docs = qopen();
					
					// Add first document
					doccount_t *dc = (doccount_t*)malloc(sizeof(doccount_t));
					if (dc != NULL) {
						dc->docID = id;
						dc->count = 1;
						qput(wn->docs, dc);
					}
					
					hput(index, wn, word, strlen(word));
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
	happly(index, free_wordnode);
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

// Search function for word in hash table
bool searchfn_word(void *elementp, const void *keyp) {
	wordnode_t *wn = (wordnode_t*)elementp;
	char *word = (char*)keyp;
	return strcmp(wn->word, word) == 0;
}

// Search function for document in queue
bool searchfn_doc(void *elementp, const void *keyp) {
	doccount_t *dc = (doccount_t*)elementp;
	int *docID = (int*)keyp;
	return dc->docID == *docID;
}

// Sum function to count all words across all documents
void sumwords_fn(void *elementp) {
	wordnode_t *wn = (wordnode_t*)elementp;
	
	// Sum counts from all documents in the queue
	queue_t *temp_queue = qopen();
	doccount_t *dc;
	
	while ((dc = (doccount_t*)qget(wn->docs)) != NULL) {
		total_words += dc->count;
		qput(temp_queue, dc);
	}
	
	// Restore queue
	while ((dc = (doccount_t*)qget(temp_queue)) != NULL) {
		qput(wn->docs, dc);
	}
	qclose(temp_queue);
}

// Free a wordnode entry
void free_wordnode(void *elementp) {
	wordnode_t *wn = (wordnode_t*)elementp;
	if (wn != NULL) {
		free(wn->word);
		
		// Free all documents in queue
		doccount_t *dc;
		while ((dc = (doccount_t*)qget(wn->docs)) != NULL) {
			free(dc);
		}
		qclose(wn->docs);
		
		free(wn);
	}
}