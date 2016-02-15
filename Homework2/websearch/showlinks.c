#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include "htmlstreamparser.h"

typedef struct {
	char * memory;
	size_t size;
} MemoryStruct;

size_t WriteToMemory(char* buffer, size_t size, size_t nmemb, MemoryStruct* memory) {
	const size_t real_size = size * nmemb;

	// Grow the buffer
	memory->memory = (char*) realloc(memory->memory, memory->size + real_size + 1);
	if (!memory->memory) {
		// Out of memory
		printf("Out of memory");
		return 0;
	}
	memcpy(memory->memory + memory->size, buffer, real_size);

	memory->size = memory->size + real_size;
	memory->memory[memory->size] = '\0';

	return real_size;
}

// Get a webpage as a string.
// The resulting pointer needs to be freed.
char * GetWebPage(const char* myurl) {
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

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage: %s URL\n", argv[0]);
		return 1;
	}
	char* page = GetWebPage(argv[1]);
	puts(page);
	free(page);
	return 0;
}
