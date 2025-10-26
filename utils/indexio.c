/* 
 * indexio.c --- 
 * 
 * Author: Magpie N. Carino
 * Created: 10-25-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#define _POSIX_C_SOURCE 200809L

#include <hash.h>
#include <queue.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static FILE *fp;

void write_doccount(void *ep) {
	doccount_t *cp = ep;
	
	fprintf(fp, " %d %d", cp->docID, cp->count);
}

void write_wordnode(void *ep) {
	wordnode_t *node = ep;
	
	fprintf(fp, "%s", node->word);
	qapply(node->docs, write_doccount);
	fprintf(fp, "\n");
}

/*
 * indexsave -- save an index <index> from indexer in a file <indexnm>
 *
 * returns 0 on success, nonzero otherwise
 *
 * file format for the index file looks like this:
 *   <word> <docID1> <count1> <docID2> <count2> ... <docIDN> <countN>
 * 
 */
int indexsave(hashtable_t *index, char *indexnm) {
	char filepath[256];
	
	// Check arguments
	if (index == NULL || indexnm  == NULL) {
		return -1;
	}
	
	// Create filepath: dirname/id
	sprintf(filepath, "%s", indexnm);
	
	// Open file for writing
	fp = fopen(filepath, "w");
	if (fp == NULL) {
		fprintf(stderr, "Error: cannot open file %s\n", filepath);
		return -1;
	}

	happly(index, write_wordnode);

	fclose(fp);
	fp = NULL;
	
	return 0;
}

/*
 * indexload -- loads the index file in <indexnm> into a new index
 *
 * returns non-NULL hashtable on success, NULL otherwise
 */
hashtable_t *indexload(char *indexnm) {
	FILE *fp;
	char word[128];
	int docID, count;

	if (indexnm == NULL)
		return NULL;

	fp = fopen(indexnm, "r");
	if (fp == NULL) {
		fprintf(stderr, "Error: cannot open file %s\n", indexnm);
		return NULL;
	}

	hashtable_t *index = hopen(100);
	if (index == NULL) {
		fclose(fp);
		return NULL;
	}

	// Read one word per line, followed by (docID, count) pairs
	while (fscanf(fp, "%127s", word) == 1) {
		wordnode_t *node = malloc(sizeof(wordnode_t));
		if (node == NULL) {
			fprintf(stderr, "Error: malloc failed for wordnode\n");
			hclose(index);
			fclose(fp);
			return NULL;
		}

		node->word = strdup(word);
		node->docs = qopen();

		// Read pairs until newline or EOF
		int c;
		while ((c = fgetc(fp)) != '\n' && c != EOF) {
			ungetc(c, fp);
			if (fscanf(fp, "%d %d", &docID, &count) == 2) {
				doccount_t *dc = malloc(sizeof(doccount_t));
				dc->docID = docID;
				dc->count = count;
				qput(node->docs, dc);
			} else {
				break;
			}
		}

		// Insert node into hashtable
		hput(index, node, node->word, strlen(node->word));
	}

	fclose(fp);
	return index;
}
