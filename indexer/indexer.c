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
#include <pageio.h>
#include <webpage.h>

int NormalizeWord(char *w);

int main(void){
	int pos = 0;
	char *word;
	int id = 1;
	char *pagedir = "../crawler/pages";
	
	webpage_t *webpage = pageload(id, pagedir);

	while ((pos = webpage_getNextWord(webpage, pos, &word)) > 0 ) {
		if (NormalizeWord(word) == 0) {
			fprintf(stdout, "%s\n", word);
		}
		free(word);
	}
	
	webpage_delete(webpage);					

	exit(EXIT_SUCCESS);
}

int NormalizeWord(char *wp) {
  if (wp == NULL || strlen(wp) < 3) return 1;

	char *p = wp;

	while (*p != '\0') {
    if (!isalpha((unsigned char) *p)) {
      return 1;
    }
    *p = tolower((unsigned char) *p);
		p++;
  }
	
  return 0;
}
