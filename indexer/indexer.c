/* 
 * indexer.c --- 
 * 
 * 
 * Description: Indexing
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "pageio.h"

int main(void){
	int pos=0;
	char *word;
	pageload(1, "pages");
	while(pos = webpage_getNextWord(page, pos, &word) > 0 ){
		fprintf(stdout, "%s\n", word); 
	}
						
	return 0;
}
