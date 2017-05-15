#ifndef WEBCLIENT_HPP
#define WEBCLIENT_HPP

#include <string>
#include <sstream>
#include <curl/curl.h>
#include "json/json.h"

class Webclient
{
	public:
		Webclient();
		~Webclient();

		Json::Value getURL(const std::string& url, long modified = -1);

		// Something went wrong while crawling
		struct CrawlException {};
		// The data is in the cache, so you should load it from there.
		struct CachedException{};

	private:
		CURL* curl;
		std::stringstream buffer;

		static size_t curlCallback(char* buffer, size_t size, size_t nmemb, Webclient* data);
};

#endif
