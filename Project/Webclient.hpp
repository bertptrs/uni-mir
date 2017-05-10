#ifndef WEBCLIENT_HPP
#define WEBCLIENT_HPP

#include <string>
#include <sstream>
#include <curl/curl.h>
#include <json/json.h>

class Webclient
{
	public:
		Webclient();
		~Webclient();

		// Request the given URL. The url is modified to reflect the actual URL.
		Json::Value getURL(const std::string& url);

		struct CrawlException {};

	private:
		CURL* curl;
		std::stringstream buffer;

		static size_t curlCallback(char* buffer, size_t size, size_t nmemb, Webclient* data);
};

#endif
