/* 
 * query.c --- 
 * 
 * Author: Eigen H. Schinaman
 * Created: 10-28-2025
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <hash.h>
#include <queue.h>
#include <indexio.h>
#include <stdlib.h>
#include <limits.h>

int min_int(int a, int b);
bool index_search(void* elementp, const void* searchkeyp);
bool queue_search(void* elementp, const void* searchkeyp);
void free_wordnode(void *elementp);

int main(void) {
	char query[1000];
	char words[40][1000];
	char result[1000];
	
	char *indexnm = "../indexer/saved_index";
	hashtable_t *index = indexload(indexnm);
	if (index == NULL) {
		fprintf(stderr, "Error: failed to load index from %s\n", indexnm);
		return EXIT_FAILURE;
	}
	
	int rank;

	while (true) {
		rank = INT_MAX;
		result[0] = '\0';
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
			
			// Search for each word and build result
			for (int i = 0; i < w; i++) {
				int docnumber = 1;
				char *word = words[i];
				
				wordnode_t *np = (wordnode_t*)hsearch(index, index_search, word, strlen(word));

				if (np == NULL) {
					sprintf(result + strlen(result), "%s:0 ", word);
					rank = 0;
					continue;
				}
				
				queue_t *doclist = np->docs;
				doccount_t *cp = (doccount_t*)qsearch(doclist, queue_search, &docnumber);
				
				if (cp == NULL) {
					sprintf(result + strlen(result), "%s:0 ", word);
					rank = 0;
					continue;
				}
				
				int count = cp->count;
				rank = min_int(count, rank);
				sprintf(result + strlen(result), "%s:%d ", word, count);
			}
			
			sprintf(result + strlen(result), "-- %d", rank);
			printf("%s\n", result);
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