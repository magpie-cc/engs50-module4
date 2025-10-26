#pragma once
/* 
 * indexio.h --- 
 * 
 * Author: Magpie N. Carino
 * Created: 10-25-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <hash.h>
#include <queue.h>

typedef struct doccount {
	int docID;
	int count;
} doccount_t;

// Structure to hold a word and its queue of documents
typedef struct wordnode {
	char *word;
	queue_t *docs;  // queue of doccount_t
} wordnode_t;

/*
 * indexsave -- save an index <index> from indexer in a file <indexnm>
 *
 * returns 0 on success, nonzero otherwise
 *
 * file format for the index file looks like this:
 *   <word> <docID1> <count1> <docID2> <count2> ... <docIDN> <countN>
 * 
 */
int indexsave(hashtable_t *index, char *indexnm);

/*
 * indexload -- loads the index file in <indexnm> into a new index
 *
 * returns non-NULL hashtable on success, NULL otherwise
 */
hashtable_t *indexload(char *indexnm);
