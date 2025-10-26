/* 
 * test_indexio.c --- 
 * 
 * Author: Magpie N. Carino
 * Created: 10-26-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <indexio.h>
#include <stdio.h>
#include <stdlib.h>

void free_doccount(void *dp) {
	free(dp);
}
       

void free_wordnode(void *ep) {
    if (ep == NULL) return;
    wordnode_t *node = ep;

    // Free the word string
    if (node->word != NULL)
        free(node->word);

    // Free each doccount_t element in the docs queue
    if (node->docs != NULL) {
        qapply(node->docs, free_doccount);
        qclose(node->docs);
    }

    // Free the wordnode itself
    free(node);
}

int main() {

	char *indexnm = "test_files/index";
	char *copynm = "test_files/index_copy";

	hashtable_t *index = indexload(indexnm);
	if (indexsave(index, copynm) != 0) {
		exit(EXIT_FAILURE);
	}
	hashtable_t *index_copy = indexload(copynm);

	happly(index, free_wordnode);
	happly(index_copy, free_wordnode);
	
	hclose(index);
	hclose(index_copy);
	
	exit(EXIT_FAILURE);
}
