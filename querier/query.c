/* 
 * query.c --- 
 * 
 * Author: Eigen H. Schinaman
 * Created: 10-28-2025
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void){
	char t[1000];
	char c[20][100];
	while(1){
		fprintf(stdout, "\n>");
		int res=scanf("%s", t);
		fprintf(stdout, "\n");
		if(res == EOF)break;
		int k=0;
		int f=0;
		while(t[k] != '\0'){
			if(isalpha(t[k])){
				c[k][f]=t[k];
			}else if(k+f != 0 && isspace(t[k]) && !isspace(t[k-1]) && t[k+1] != '\0'){
				c[k][f] = ' ';
				f++;
			}else{
				fprintf(stdout, "[invalid query]");
				break;
			}
			k++;
		}
		c[k+f+1]=' ';
		fprintf(stdout, "%s", c);
	}
	return 0;
}
