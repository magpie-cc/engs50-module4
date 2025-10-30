/* 
 * query.c - Module 6 Step 3: Rank N-Documents
 * 
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

// Structure to hold document rank
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

int main(void) {
	char query[1000];
	char words[40][1000];
	
	char *indexnm = "../indexer/index_step3";
	char *pagedir = "../pages_step3";
	
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
		int w = 0, j = 0, invalid = 0;

		// Parse query into words
		for (int i = 0; query[i] != '\0'; i++) {
			if (isalpha(query[i])) {
				words[w][j++] = tolower(query[i]);
			}
			else if (isspace(query[i]) || query[i] == '\n') {
				if (j > 0) {
					words[w][j] = '\0';
					w++;
					j = 0;
				}
			}
			else {
				fprintf(stdout, "[invalid query]\n");
				invalid = 1;
				break;
			}
		}
		
		if (j > 0)
			words[w++][j] = '\0';
		
		// Skip empty queries
		if (w == 0 && !invalid) {
			continue;
		}
		
		// Process valid queries
		if (!invalid) {
			// Print normalized query
			for (int i = 0; i < w; i++) {
				printf("%s ", words[i]);
			}
			printf("\n");
			
			// Create queue to hold results
			queue_t *results = qopen();
			
			// Get first word's document list
			char *firstword = words[0];
			wordnode_t *firstnode = (wordnode_t*)hsearch(index, index_search, firstword, strlen(firstword));
			
			if (firstnode == NULL) {
				printf("No documents found\n");
				qclose(results);
				continue;
			}
			
			// For each document in first word's list
			queue_t *temp = qopen();
			doccount_t *dc;
			
			// Copy first word's docs to temp queue
			while ((dc = (doccount_t*)qget(firstnode->docs)) != NULL) {
				qput(temp, dc);
			}
			// Restore
			while ((dc = (doccount_t*)qget(temp)) != NULL) {
				qput(firstnode->docs, dc);
				
				int docID = dc->docID;
				int rank = dc->count;  // Start with first word count
				
				// Check if this document contains ALL other words
				bool hasAllWords = true;
				for (int i = 1; i < w; i++) {
					wordnode_t *wn = (wordnode_t*)hsearch(index, index_search, words[i], strlen(words[i]));
					if (wn == NULL) {
						hasAllWords = false;
						break;
					}
					
					doccount_t *doc = (doccount_t*)qsearch(wn->docs, queue_search, &docID);
					if (doc == NULL) {
						hasAllWords = false;
						break;
					}
					
					// Update rank (minimum)
					rank = min_int(rank, doc->count);
				}
				
				// If document has all words, add to results
				if (hasAllWords) {
					docrank_t *dr = (docrank_t*)malloc(sizeof(docrank_t));
					if (dr != NULL) {
						dr->docID = docID;
						dr->rank = rank;
						dr->url = get_url(docID, pagedir);
						qput(results, dr);
					}
				}
			}
			qclose(temp);
			
			// Print results
			docrank_t *result;
			while ((result = (docrank_t*)qget(results)) != NULL) {
				printf("rank: %d: doc: %d: %s\n", result->rank, result->docID, result->url);
				free_docrank(result);
			}
			
			qclose(results);
		}
	}

	// Cleanup
	happly(index, free_wordnode);
	hclose(index);
	
	return 0;
}

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