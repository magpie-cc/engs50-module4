/* 
 * indexer.c - Complete indexer for Module 5 Step 7
 * 
 * Description: Index webpages from crawler directory
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
#include <indexio.h>
#include <dirent.h>
#include <errno.h>

// Function prototypes
int NormalizeWord(char *w);
bool searchfn_word(void *elementp, const void *keyp);
bool searchfn_doc(void *elementp, const void *keyp);
void free_wordnode(void *elementp);

int main(int argc, char *argv[]) {
	const uint32_t HASHTABLE_SIZE = 100;

	// Validate arguments
	if (argc != 3) {
		fprintf(stderr, "usage: indexer <pagedir> <indexnm>\n");
		return EXIT_FAILURE;
	}
	
	char *pagedir = argv[1];
	char *indexnm = argv[2];
	
	// Verify directory exists
	DIR *dir = opendir(pagedir);
	if (!dir) {
		fprintf(stderr, "Error: cannot open directory %s\n", pagedir);
		return EXIT_FAILURE;
	}
	
	printf("Indexer: Processing pages from %s\n", pagedir);
	printf("============================\n\n");
	
	// Create hash table
	hashtable_t *index = hopen(HASHTABLE_SIZE);
	if (index == NULL) {
		fprintf(stderr, "Error: failed to create hash table\n");
		closedir(dir);
		return EXIT_FAILURE;
	}

	// Process all pages in directory
	struct dirent *entry;
	int pages_processed = 0;
	
	while ((entry = readdir(dir)) != NULL) {
		// Skip . and .. and non-numeric entries
		if (entry->d_name[0] == '.' || !isdigit(entry->d_name[0])) {
			continue;
		}
		
		int current_id = atoi(entry->d_name);
		if (current_id == 0) {  // Skip invalid conversions
			continue;
		}
		
		webpage_t *webpage = pageload(current_id, pagedir);
		if (webpage == NULL) {
			fprintf(stderr, "Warning: failed to load page %d\n", current_id);
			continue;
		}
		
		printf("Processing page %d: %s\n", current_id, webpage_getURL(webpage));
		pages_processed++;
		
		int pos = 0;
		char *word;
		
		// Extract words and build index
		while ((pos = webpage_getNextWord(webpage, pos, &word)) > 0) {
			// Normalize word
			if (NormalizeWord(word) == 0) {
				// Search for word in hash table
				wordnode_t *wn = (wordnode_t*)hsearch(index, searchfn_word, word, strlen(word));
				
				if (wn != NULL) {
					// Word found - search for document in queue
					doccount_t *dc = (doccount_t*)qsearch(wn->docs, searchfn_doc, &current_id);
					
					if (dc != NULL) {
						// Document found - increment count
						dc->count++;
					} else {
						// Document not found - create new entry
						dc = (doccount_t*)malloc(sizeof(doccount_t));
						if (dc != NULL) {
							dc->docID = current_id;
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
							dc->docID = current_id;
							dc->count = 1;
							qput(wn->docs, dc);
						}
						
						hput(index, wn, word, strlen(word));
					}
				}
			}
			free(word);
		}
		
		webpage_delete(webpage);
	}
	
	closedir(dir);
	
	printf("\nProcessed %d pages\n", pages_processed);
	printf("Saving index to %s...\n", indexnm);
	
	// Save index
	if (indexsave(index, indexnm) != 0) {
		fprintf(stderr, "Error: failed to save index\n");
		happly(index, free_wordnode);
		hclose(index);
		return EXIT_FAILURE;
	}
	
	printf("Index saved successfully!\n");
	
	// Cleanup
	happly(index, free_wordnode);
	hclose(index);
	
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