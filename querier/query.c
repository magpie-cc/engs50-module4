/* 
 * query.c --- 
 * 
 * Author: Eigen H. Schinaman
 * Created: 10-28-2025
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void) {
    char t[1000];
    char c[100][40];
    while (1) {
			fprintf(stdout, "\n>");
        if (!fgets(t, sizeof(t), stdin))
            break;
				t[strcspn(t, "\n")] = '\0';
        int w=0, j=0, k=0;
        for (int i = 0; t[i] != '\0'; i++) {
            if(isalpha(t[i])) {
							if(isupper(t[i])){
									c[w][j++] = t[i]+32;
							}else c[w][j++] = t[i];
            } else if(isspace(t[i]) || t[i] == '\n') {
                if (j > 0) {
                    c[w][j] = '\0';
                    w++;
                    j = 0;
                }
            } else {
							fprintf(stdout, "[invalid query]\n");
								k=1;
                break;
            }
        }
        if (j > 0)
            c[w++][j] = '\0';

				if(!k){
					for (int i = 0; i < w; i++){
						fprintf(stdout, "%s ", c[i]);
					}
					fprintf(stdout, "\n");
				}
    }
    return 0;
}

