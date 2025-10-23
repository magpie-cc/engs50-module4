/* 
11;rgb:3030/0a0a/2424* crawler.c - web crawler for tiny search engine
 * Step 6: Complete Crawler
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>     
#include "webpage.h"
#include "queue.h"
#include "hash.h"

// Function prototypes
int32_t pagesave(webpage_t *pagep, int id, char *dirname);

// Helper function to print a webpage from the queue or from the hash table
void print_webpage(void *elementp) {
	webpage_t *page = (webpage_t*)elementp;
	printf("  URL: %s (depth: %d)\n", webpage_getURL(page), webpage_getDepth(page));
}

// helper search function
bool searchfn(void *elementp, const void *searchkeyp) {
	return strcmp(elementp, searchkeyp) == 0;
}

// helper function to check if a url is visited
bool visited_url(hashtable_t *hp, char *url) {
	return hsearch(hp, &searchfn, url, strlen(url)) != NULL;
}

// helper function to free a url string
void free_url(void *urlp) {
	if (urlp != NULL) {
		free((char*)urlp);
	}
}

// helper function to remove trailing frontslash in url
void remove_trailing_slash(char *str) {
    int len = strlen(str);

    if (len > 0 && str[len - 1] == '/') {
        str[len - 1] = '\0'; 
    }
}

/* 
 * pagesave - saves a webpage to a file
 * @pagep: pointer to webpage to save
 * @id: unique id number for the file
 * @dirname: directory to save the file in
 * 
 * Returns 0 on success, -1 on failure
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

int main(int argc, char* argv[]) {
	webpage_t *page;
	queue_t *queue;
	hashtable_t *hashtable;
	const uint32_t HASHTABLE_SIZE = 20;
	int pageIndex = 1;
	
	if (argc != 4) {
		fprintf(stderr, "usage: crawler <seedurl> <pagedir> <maxdepth>\n");
		return EXIT_FAILURE;
	}

	// crawler "https://thayer.github.io/engs50/" ../pages 1
	char *seedURL = argv[1];
	char *pagedir = argv[2];
	int max_depth = strtoul(argv[3], NULL, 10);
	
	// Create queue for internal URLs
	queue = qopen();
	if (queue == NULL) {
		fprintf(stderr, "Error: failed to create queue\n");
		return EXIT_FAILURE;
	}

	// Create hashtable to record visited URLs
	hashtable = hopen(HASHTABLE_SIZE);
	if (hashtable == NULL) {
		fprintf(stderr, "Error: failed to create hashtable\n");
		return EXIT_FAILURE;
	}

	printf("==========================\n\n");
	
	printf("Fetching seed page: %s\n", seedURL);
	
	// Create webpage at depth 0
	page = webpage_new( (char*) seedURL, 0, NULL);
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

	
	// initial load
	// put page in queue, mark as seen in hashtable, and save page
	qput(queue, page);
	char *seedcopy = strdup(seedURL);
	hput(hashtable, seedcopy, seedcopy, strlen(seedcopy));
	pagesave(page, pageIndex++, (char*)pagedir);
	printf("\n");
	//char *seedcopycopy = strdup(seedcopy);
	//remove_trailing_slash(seedcopycopy);
	//hput(hashtable, seedcopycopy, seedcopycopy, strlen(seedcopycopy));
	// Extract URLs and add internal ones to queue
	printf("Extracting URLs...\n");

	while ((page = qget(queue)) != NULL) {
		int pos = 0;
		char *url;
		
		// Extract URLs from this page
		while ((pos = webpage_getNextURL(page, pos, &url)) > 0) {
			// Check if we've reached max depth - if so, free URL and skip processing
			if (webpage_getDepth(page) >= max_depth) {
				free(url);
				continue;
			}
			
			// Check if internal
			if (IsInternalURL(url)) {
				printf("  [INTERNAL] %s\n", url);
				// Check if already visited
				if (!visited_url(hashtable, url)) {
					// Create new webpage for this URL at depth+1
					webpage_t *new_page = webpage_new(url, webpage_getDepth(page) + 1, NULL);
					if (new_page != NULL && webpage_fetch(new_page)) {
						qput(queue, new_page);
						char *urlcopy = strdup(url);      
						hput(hashtable, urlcopy, urlcopy, strlen(urlcopy));
						if (pagesave(new_page, pageIndex++, (char*)pagedir) != 0) {
							fprintf(stderr, "Error: failed to save page\n");
						}
						//char *urlcopycopy = strdup(urlcopy);      
						//remove_trailing_slash(urlcopycopy);
						//hput(hashtable, urlcopycopy, urlcopycopy, strlen(urlcopycopy));
					}
				}
				free(url);
			} else {
				printf("  [EXTERNAL] %s (skipped)\n", url);
				free(url);
			}
		}
		webpage_delete(page);
	}

	// Clean up: remove and delete all webpages from queue
	webpage_t *wp;
	while ((wp = (webpage_t*)qget(queue)) != NULL) {
		webpage_delete(wp);
	}
	
	qclose(queue);
	happly(hashtable, free_url);
	hclose(hashtable);
	
	return EXIT_SUCCESS;
}
