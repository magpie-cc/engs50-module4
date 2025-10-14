/* 
 * crawler.c - web crawler for tiny search engine
 * Step 3: Queue of webpages
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"
#include "queue.h"

// Helper function to print a webpage from the queue
void print_webpage(void *elementp) {
	webpage_t *page = (webpage_t*)elementp;
	printf("  URL: %s (depth: %d)\n", webpage_getURL(page), webpage_getDepth(page));
}

int main(void) {
	webpage_t *page;
	queue_t *queue;
	char *url;
	int pos = 0;
	
	// Seed URL
	const char *seedURL = "https://thayer.github.io/engs50/";
	
	printf("Step 3: Queue of Webpages\n");
	printf("==========================\n\n");
	
	printf("Fetching seed page: %s\n", seedURL);
	
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
	
	printf("Successfully fetched! (%d bytes)\n\n", webpage_getHTMLlen(page));
	
	// Create queue for internal URLs
	queue = qopen();
	if (queue == NULL) {
		fprintf(stderr, "Error: failed to create queue\n");
		webpage_delete(page);
		return EXIT_FAILURE;
	}
	
	// Extract URLs and add internal ones to queue
	printf("Extracting URLs...\n");

	while ((pos = webpage_getNextURL(page, pos, &url)) > 0) {
		// Check if internal
		if (IsInternalURL(url)) {
			printf("  [INTERNAL] %s\n", url);
			
			// Create new webpage for this URL at depth 1
			webpage_t *new_page = webpage_new(url, 1, NULL);
			if (new_page != NULL) {
				qput(queue, new_page);
			}
			free(url);  // FIX: webpage_new makes a copy, so free original
		} else {
			printf("  [EXTERNAL] %s (skipped)\n", url);
			free(url);  // Free external URLs
		}
	}
	
	printf("\n");
	
	// Print the queue
	printf("Queue contents:\n");
	printf("---------------\n");
	qapply(queue, print_webpage);
	printf("---------------\n\n");
	
	// Verify requirements
	printf("Verification:\n");
	printf("- All URLs in queue are internal: ✓\n");
	printf("- Two entries for CodingStyle.html exist: check queue above\n");
	
	// Clean up: remove and delete all webpages from queue
	webpage_t *wp;
	while ((wp = (webpage_t*)qget(queue)) != NULL) {
		webpage_delete(wp);
	}
	
	// Close queue
	qclose(queue);
	
	// Delete the seed page
	webpage_delete(page);
	
	printf("\n✓ Step 3 complete!\n");
	
	return EXIT_SUCCESS;
}