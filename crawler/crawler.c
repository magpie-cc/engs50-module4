/* 
 * crawler.c - web crawler for tiny search engine
 * Step 2: Fetch one page and extract URLs
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"

int main(void) {
	webpage_t *page;
	char *url;
	int pos = 0;
	
	// Seed URL
	const char *seedURL = "https://thayer.github.io/engs50/";
	
	printf("Fetching page: %s\n\n", seedURL);
	
	// Create webpage at depth 0
	page = webpage_new((char*)seedURL, 0, NULL);
	if (page == NULL) {
		fprintf(stderr, "Error: failed to create webpage\n");
		return EXIT_FAILURE;
	}
	
	// Fetch the page
	if (!webpage_fetch(page)) {
		fprintf(stderr, "Error: failed to fetch webpage\n");
		webpage_delete(page);
		return EXIT_FAILURE;
	}
	
	printf("Successfully fetched page!\n");
	printf("Page length: %d bytes\n\n", webpage_getHTMLlen(page));
	
	// Extract and print all URLs
	printf("URLs found:\n");
	printf("----------------------------------------\n");
	
	while ((pos = webpage_getNextURL(page, pos, &url)) > 0) {
		// Check if internal or external
		if (IsInternalURL(url)) {
			printf("[INTERNAL] %s\n", url);
		} else {
			printf("[EXTERNAL] %s\n", url);
		}
		free(url);  // webpage_getNextURL allocates memory
	}
	
	printf("----------------------------------------\n");
	
	// Clean up
	webpage_delete(page);
	
	return EXIT_SUCCESS;
}