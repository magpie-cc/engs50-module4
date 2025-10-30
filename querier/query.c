/* 
 * query.c - Module 6 Step 4: Complex Queries with AND/OR
 */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <hash.h>
#include <queue.h>
#include <pageio.h>
#include <webpage.h>
#include <indexio.h>
#include <stdlib.h>
#include <limits.h>

// Structures
typedef struct docrank {
	int docID;
	int rank;
	char *url;
} docrank_t;

// Function prototypes
int min_int(int a, int b);
bool index_search(void* elementp, const void* searchkeyp);
bool queue_search(void* elementp, const void* searchkeyp);
bool docrank_search(void* elementp, const void* searchkeyp);
void free_wordnode(void *elementp);
void free_docrank(void *elementp);
char* get_url(int docID, char *pagedir);
bool validate_query(char **words, int count);
queue_t* process_and_sequence(char **words, int start, int end, hashtable_t *index, char *pagedir);
void merge_or_results(queue_t *results, queue_t *new_results);

// Globals
char *pagedir_global;

int main(void) {
	char query[1000];
	char *words[100];  // Pointers to words
	
	char *indexnm = "../indexer/index_step4";
	pagedir_global = "../pages_step4";
	
	hashtable_t *index = indexload(indexnm);
	if (index == NULL) {
		fprintf(stderr, "Error: failed to load index from %s\n", indexnm);
		return EXIT_FAILURE;
	}

	while (true) {
		fprintf(stdout, "\n> ");
		fflush(stdout);
		
		if (!fgets(query, sizeof(query), stdin))
			break;
		
		query[strcspn(query, "\n")] = '\0';
		
		// Tokenize query into words
		int w = 0;
		char *token = strtok(query, " \t");
		while (token != NULL && w < 100) {
			// Check for invalid characters
			bool valid = true;
			for (int i = 0; token[i] != '\0'; i++) {
				if (!isalpha(token[i])) {
					printf("[invalid query]\n");
					valid = false;
					break;
				}
			}
			
			if (!valid) {
				w = -1;  // Mark as invalid
				break;
			}
			
			// Convert to lowercase
			for (int i = 0; token[i] != '\0'; i++) {
				token[i] = tolower(token[i]);
			}
			
			words[w++] = token;
			token = strtok(NULL, " \t");
		}
		
		// Skip if invalid or empty
		if (w == -1) continue;
		if (w == 0) continue;
		
		// Validate query syntax
		if (!validate_query(words, w)) {
			printf("[invalid query]\n");
			continue;
		}
		
		// Print normalized query
		for (int i = 0; i < w; i++) {
			printf("%s ", words[i]);
		}
		printf("\n");
		
		// Process query: split by OR, process each AND sequence
		queue_t *final_results = qopen();
		int seq_start = 0;
		
		for (int i = 0; i <= w; i++) {
			if (i == w || strcmp(words[i], "or") == 0) {
				// Process AND sequence from seq_start to i-1
				if (i > seq_start) {
					queue_t *seq_results = process_and_sequence(words, seq_start, i, index, pagedir_global);
					merge_or_results(final_results, seq_results);
					qclose(seq_results);
				}
				seq_start = i + 1;
			}
		}
		
		// Print results
		docrank_t *first = (docrank_t*)qget(final_results);
		if (first == NULL) {
			printf("No documents found\n");
		} else {
			// Print first result
			printf("rank: %d: doc: %d: %s\n", first->rank, first->docID, first->url);
			free_docrank(first);
			
			// Print remaining results
			docrank_t *dr;
			while ((dr = (docrank_t*)qget(final_results)) != NULL) {
				printf("rank: %d: doc: %d: %s\n", dr->rank, dr->docID, dr->url);
				free_docrank(dr);
			}
		}
		
		qclose(final_results);
	}

	// Cleanup
	happly(index, free_wordnode);
	hclose(index);
	
	return 0;
}

// Validate query syntax
bool validate_query(char **words, int count) {
	if (count == 0) return false;
	
	// Check first word
	if (strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0) {
		return false;
	}
	
	// Check last word
	if (strcmp(words[count-1], "and") == 0 || strcmp(words[count-1], "or") == 0) {
		return false;
	}
	
	// Check for adjacent operators
	for (int i = 0; i < count - 1; i++) {
		bool is_op1 = (strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0);
		bool is_op2 = (strcmp(words[i+1], "and") == 0 || strcmp(words[i+1], "or") == 0);
		
		if (is_op1 && is_op2) {
			return false;
		}
	}
	
	return true;
}

// Process AND sequence (words from start to end-1, skipping "and")
queue_t* process_and_sequence(char **words, int start, int end, hashtable_t *index, char *pagedir) {
	queue_t *results = qopen();
	
	// Collect actual words (skip "and")
	char *and_words[100];
	int and_count = 0;
	for (int i = start; i < end; i++) {
		if (strcmp(words[i], "and") != 0) {
			and_words[and_count++] = words[i];
		}
	}
	
	if (and_count == 0) return results;
	
	// Get first word's documents
	wordnode_t *firstnode = (wordnode_t*)hsearch(index, index_search, and_words[0], strlen(and_words[0]));
	if (firstnode == NULL) return results;
	
	// For each document in first word
	queue_t *temp = qopen();
	doccount_t *dc;
	
	while ((dc = (doccount_t*)qget(firstnode->docs)) != NULL) {
		qput(temp, dc);
	}
	while ((dc = (doccount_t*)qget(temp)) != NULL) {
		qput(firstnode->docs, dc);
		
		int docID = dc->docID;
		int rank = dc->count;
		bool hasAll = true;
		
		// Check all other words
		for (int i = 1; i < and_count; i++) {
			wordnode_t *wn = (wordnode_t*)hsearch(index, index_search, and_words[i], strlen(and_words[i]));
			if (wn == NULL) {
				hasAll = false;
				break;
			}
			
			doccount_t *doc = (doccount_t*)qsearch(wn->docs, queue_search, &docID);
			if (doc == NULL) {
				hasAll = false;
				break;
			}
			
			rank = min_int(rank, doc->count);
		}
		
		if (hasAll) {
			docrank_t *dr = (docrank_t*)malloc(sizeof(docrank_t));
			if (dr) {
				dr->docID = docID;
				dr->rank = rank;
				dr->url = get_url(docID, pagedir);
				qput(results, dr);
			}
		}
	}
	qclose(temp);
	
	return results;
}

// Merge OR results (add ranks for same doc, add new docs)
void merge_or_results(queue_t *results, queue_t *new_results) {
	docrank_t *new_dr;
	
	while ((new_dr = (docrank_t*)qget(new_results)) != NULL) {
		// Check if document already in results
		docrank_t *existing = (docrank_t*)qsearch(results, docrank_search, &(new_dr->docID));
		
		if (existing) {
			// Add ranks
			existing->rank += new_dr->rank;
			free_docrank(new_dr);
		} else {
			// Add new document
			qput(results, new_dr);
		}
	}
}

// Helper functions
int min_int(int a, int b) {
	return (a < b) ? a : b;
}

bool index_search(void* elementp, const void* searchkeyp) {
	wordnode_t *np = (wordnode_t*)elementp;
	char *wp = (char*)searchkeyp;
	return strcmp(wp, np->word) == 0;
}

bool queue_search(void* elementp, const void* searchkeyp) {
	doccount_t *cp = (doccount_t*)elementp;
	int docID = *(const int*)searchkeyp;
	return docID == cp->docID;
}

bool docrank_search(void* elementp, const void* searchkeyp) {
	docrank_t *dr = (docrank_t*)elementp;
	int docID = *(const int*)searchkeyp;
	return dr->docID == docID;
}

char* get_url(int docID, char *pagedir) {
	webpage_t *page = pageload(docID, pagedir);
	if (page == NULL) {
		return strdup("unknown");
	}
	char *url = strdup(webpage_getURL(page));
	webpage_delete(page);
	return url;
}

void free_wordnode(void *elementp) {
	wordnode_t *wn = (wordnode_t*)elementp;
	if (wn != NULL) {
		free(wn->word);
		doccount_t *dc;
		while ((dc = (doccount_t*)qget(wn->docs)) != NULL) {
			free(dc);
		}
		qclose(wn->docs);
		free(wn);
	}
}

void free_docrank(void *elementp) {
	docrank_t *dr = (docrank_t*)elementp;
	if (dr != NULL) {
		free(dr->url);
		free(dr);
	}
}