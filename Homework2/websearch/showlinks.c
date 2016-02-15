#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include "htmlstreamparser.h"

typedef struct {
	char * memory;
	size_t size;
} MemoryStruct;

// CURL write callback that writes to memory.
//
// Based on: https://curl.haxx.se/libcurl/c/getinmemory.html
size_t WriteToMemory(char* buffer, size_t size, size_t nmemb, MemoryStruct* memory)
{
	const size_t real_size = size * nmemb;

	// Grow the buffer
	memory->memory = (char*) realloc(memory->memory, memory->size + real_size + 1);
	// Append the data
	memcpy(memory->memory + memory->size, buffer, real_size);
	memory->size += real_size;
	// Set null terminator
	memory->memory[memory->size] = '\0';

	return real_size;
}


// Get a webpage as a string.
// The resulting pointer needs to be freed.
char * GetWebPage(const char* myurl)
{
	CURL* curl = curl_easy_init();
	MemoryStruct memory = { malloc(1), 0 };

	// Initialize CURL
	curl_easy_setopt(curl, CURLOPT_URL, myurl);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

	// Initialize our callback
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToMemory);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &memory);

	CURLcode res = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	if (res) {
		// CURL failed
		free(memory.memory);

		return NULL;
	} else {
		return memory.memory;
	}
}

// Create an absolute link from a relative one.
//
// This method returns a string pointer that must be freed.
char* FormatLink(char* url, char* link)
{
	char* result;

	if (strstr(link, "://")) {
		// Link contains protocol. Assume full link.
		result = malloc(strlen(link) + 1);
		strcpy(result, link);
		return result;
	}

	// Link is at least somewhat relative, compute domain.
	char* domainBegin = strstr(url, "://") + 3;
	char* domainEnd = strrchr(domainBegin, '/');
	if (domainEnd == NULL) {
		// Pure domain without trailing slash.
		domainEnd = domainBegin + strlen(domainBegin);
	} else {
		// Skip the trailing slash.
		domainEnd--;
	}
	size_t domainLength = domainEnd - url;


	if (link[0] == '/') {
		// Link is domain relative. Prepend protocol and domain
		result = malloc(strlen(link) + domainLength + 1);
		memcpy(result, url, domainLength);
		strcpy(result + domainLength, link);
	} else {
		// Link is directory relative. Compute currect directory
		char* dirEnd = strrchr(domainEnd, '/');
		size_t prefixLength;
		if (dirEnd != NULL) {
			prefixLength = dirEnd - url - 1;
		} else {
			prefixLength = domainLength;
		}

		// Allocate for prefix, '/', link, and terminator.
		result = malloc(prefixLength + 1 + strlen(link) + 1);
		memcpy(result, url, prefixLength);
		result[prefixLength] = '/';
		strcpy(result + prefixLength + 1, link);
	}
	return result;
}

char* GetLinksByTag(char* htmlcontent, char* url, char * tag_name, char * attribute)
{
	// Buffers for the stream parser.
	char tag[8];
	char attr[20];
	char val[1024];
	// Links string.
	char* links = malloc(1);
	size_t links_length = 0;
	int i; // Loop iterator
	char* base = malloc(strlen(url) + 1);

	strcpy(base, url);

	// Initialize the parser
	HTMLSTREAMPARSER *hsp = html_parser_init( );
	html_parser_set_tag_to_lower(hsp, 1);
	html_parser_set_attr_to_lower(hsp, 1);
	html_parser_set_tag_buffer(hsp, tag, sizeof(tag));
	html_parser_set_attr_buffer(hsp, attr, sizeof(attr));
	html_parser_set_val_buffer(hsp, val, sizeof(val)-1);

	links[0] = '\0';

	const size_t document_size = strlen(htmlcontent);
	const size_t tag_length = strlen(tag_name);
	const size_t attr_length = strlen(attribute);

	// Loop over document
	for (i = 0; i < document_size; i++) {
		html_parser_char_parse(hsp, htmlcontent[i]);
		// Detect different html base
		if (html_parser_cmp_tag(hsp, "base", 4) && html_parser_cmp_attr(hsp, "href", 4) && html_parser_is_in(hsp, HTML_VALUE_ENDED)) {
			html_parser_val(hsp)[html_parser_val_length(hsp)] = '\0';
			base = realloc(base, strlen(html_parser_val(hsp)) + 1);
			strcpy(base, html_parser_val(hsp));
		}
		if (html_parser_cmp_tag(hsp, tag_name, tag_length)) {
			if (html_parser_cmp_attr(hsp, attribute, attr_length)) {
				if (html_parser_is_in(hsp, HTML_VALUE_ENDED)) {
					html_parser_val(hsp)[html_parser_val_length(hsp)] = '\0';
					char* link = FormatLink(base, html_parser_val(hsp));
					// Plus 1 byte for the newline
					size_t link_length = strlen(link) + 1;
					// Plus 1 byte for the null terminator
					links = realloc(links, links_length + link_length + 1);
					strcat(links, link);
					strcat(links, "\n");
					links_length += link_length;
					free(link);
				}
			}
		}
	}

	/*release the hsp*/
	html_parser_cleanup(hsp);
	free(base);
	return links;
}

char* GetLinksFromWebPage(char* htmlcontent, char* url)
{
	return GetLinksByTag(htmlcontent, url, "a", "href");
}

char* GetImageLinksFromWebPage(char* htmlcontent, char* url)
{
	return GetLinksByTag(htmlcontent, url, "img", "src");
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage: %s URL\n", argv[0]);
		return 1;
	}
	if (strstr(argv[1], "://") == NULL) {
		printf("URL does not contain protocol: %s", argv[1]);
		return 1;
	}
	char* page = GetWebPage(argv[1]);

	char* links = GetLinksFromWebPage(page, argv[1]);
	puts(links);
	free(links);

	links = GetImageLinksFromWebPage(page, argv[1]);
	puts(links);
	free(links);

	free(page);
	return 0;
}
