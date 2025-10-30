/* 
 * query.c --- 
 * 
 * Author: Eigen H. Schinaman
 * Created: 10-28-2025
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <hash.h>
#include <indexio.h>
#include <stdlib.h>
#include <limits.h>

int min_int(int a, int b);

bool index_search(void* elementp, const void* searchkeyp);
bool queue_search(void* elementp, const void* searchkeyp);

int main(void) {
	char query[1000];
	char words[40][1000];
	char result[1000];
	
	char *indexnm = "../indexer/saved_index";
	hashtable_t *index = indexload(indexnm);
	int rank;

	while (true) {
		rank = INT_MAX;
		result[0] = '\0';
		fprintf(stdout, "\n>");
		if (!fgets(query, sizeof(query), stdin))
			break;
		
		query[strcspn(query, "\n")] = '\0';
		int w = 0, j = 0, k = 0;

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
				k = 1;
				break;
			}
		}
		
		if (j > 0)
			words[w++][j] = '\0';
		
		if (!k) {
			for (int i = 0; i < w; i++){
				int docnumber = 1;
				char *word = malloc(strlen(words[i]) + 1);
				strcpy(word, words[i]);
				
				wordnode_t *np = (wordnode_t*) hsearch(index, index_search, word, strlen(word));

				if (np == NULL) {
					sprintf(result + strlen(result), "%s:0 ", word);
					rank = 0;
					continue;
				}
				
				queue_t *doclist = np->docs;
				
				doccount_t *cp = (doccount_t*) qsearch(doclist, queue_search, &docnumber);
				int count = cp->count;

				if (cp == NULL) {
					sprintf(result + strlen(result), "%s:0 ", word);
					rank = 0;
					continue;
				} 
				
				rank = min_int(count, rank);
				sprintf(result + strlen(result), "%s:%d ", word, count);
				free(word);
			}
			sprintf(result + strlen(result), "-- %d", rank);
			printf("%s\n", result);
		}
	}

	hclose(index);
	
	return 0;
}

int min_int(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

bool index_search(void* elementp, const void* searchkeyp) {
	wordnode_t *np = (wordnode_t*) elementp;
	char *wp = (char*) searchkeyp;
	if (strcmp(wp, np->word) == 0) {
		return true;
	}
	return false;
}

bool queue_search(void* elementp, const void* searchkeyp) {
	doccount_t *cp = (doccount_t*)  elementp;
	int docID = *(const int*) searchkeyp;
	if (docID == cp->docID) {
		return true;
	}
	return false;
}

