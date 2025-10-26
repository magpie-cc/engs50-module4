/* 
 * test_pageio.c --- 
 * 
 * Author: Magpie N. Carino
 * Created: 10-23-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <pageio.h>
#include <webpage.h>
#include <stdio.h>

int main() {

	int id = 1;
	char *dirnm = "../pages";
	char *testdir = "../test_files";
		
	webpage_t *wp = pageload(id, dirnm);
	pagesave(wp, id, testdir);
	webpage_t *wp2 = pageload(id, testdir);

	printf("wp: %d, %d, %s\n", webpage_getDepth(wp), webpage_getHTMLlen(wp), webpage_getURL(wp));
	printf("wp2: %d, %d, %s\n", webpage_getDepth(wp2), webpage_getHTMLlen(wp2), webpage_getURL(wp2));

	// Add cleanup
	webpage_delete(wp);
	webpage_delete(wp2);
	
	return EXIT_SUCCESS; 

}
