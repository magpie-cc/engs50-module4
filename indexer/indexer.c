/* 
 * indexer.c --- 
 * 
 * 
 * Description: Indexing
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "pageio.h"

int NormalizeWord(char *w);

int main(void){
	int pos=0;
	char *word;
	webpage_t *page = pageload(1, "pages");
	while((pos = webpage_getNextWord(page, pos, &word)) > 0 ){
		if(NormalizeWord(word) == 0){
			fprintf(stdout, "%s\n", word);
		}
		free(word);
	}
	free(page);					
	return 0;
}

int NormalizeWord(char *w){
	int k=0;
	while(w[k] != '\0'){
		if(!isalpha(w[k])){
			return ;
		}else if(w[k] <= 'Z') w[k] +=32;
		k++;
	}
	return 0;
}
