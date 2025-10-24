/* 
 * pageio.c ---
 * 
 * Author: Magpie N. Carino
 * Created: 10-23-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <stdint.h>
#include <webpage.h>
#include <stdio.h>
#include <string.h> 

/*
 * pagesave -- save the page in filename id in directory dirnm
 *
 * returns: 0 for success; nonzero otherwise
 *
 * The suggested format for the file is:
 *   <url>
 *   <depth>
 *   <html-length>
 *   <html>
 */
int32_t pagesave(webpage_t *pagep, int id, char *dirname) {
	FILE *fp;
	char filepath[256];
	
	// Check arguments
	if (pagep == NULL || dirname == NULL || id < 0) {
		return -1;
	}
	
	// Create filepath: dirname/id
	sprintf(filepath, "%s/%d", dirname, id);
	
	// Open file for writing
	fp = fopen(filepath, "w");
	if (fp == NULL) {
		fprintf(stderr, "Error: cannot open file %s\n", filepath);
		return -1;
	}
	
	// Write URL (line 1)
	fprintf(fp, "%s\n", webpage_getURL(pagep));
	
	// Write depth (line 2)
	fprintf(fp, "%d\n", webpage_getDepth(pagep));
	
	// Write HTML length (line 3)
	fprintf(fp, "%d\n", webpage_getHTMLlen(pagep));
	
	// Write HTML content (rest of file)
	fprintf(fp, "%s", webpage_getHTML(pagep));
	
	// Close file
	fclose(fp);
	
	printf("Saved page to: %s\n", filepath);
	
	return 0;
}

/* 
 * pageload -- loads the numbered filename <id> in directory <dirnm>
 * into a new webpage
 *
 * returns: non-NULL for success; NULL otherwise
 */
webpage_t *pageload(int id, char *dirnm) {
	FILE *fp;
	char filepath[256];
	
	if (dirnm == NULL || id < 0) return NULL;
	
	sprintf(filepath, "%s/%d", dirnm, id);
	
	fp = fopen(filepath, "r");
	if (fp == NULL) {
		fprintf(stderr, "Error: cannot open file %s\n", filepath);
		return NULL;
	}

	char *url = malloc(256 * sizeof(char));
	int depth;
	int htmlsize;
	
  fscanf(fp, "%s\n%d\n%d\n", url, &depth, &htmlsize);

	char *html = malloc(htmlsize);
	if (html == NULL) return NULL;

	char ch;
	int i = 0;
  while ((ch = fgetc(fp)) != EOF) {
    html[i++] = ch;
  }

  html[i] = 0;
	
	webpage_t *wp = webpage_new(url, depth, html);
	if (wp == NULL) return NULL;
	
	return wp;
}


