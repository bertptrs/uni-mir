#ifndef WEBCLIENT_HPP
#define WEBCLIENT_HPP

#include <string>
#include <sstream>
#include <curl/curl.h>

class Webclient
{
	public:
		Webclient();
		~Webclient();

		std::string getURL(const std::string& url);

		struct CrawlException {};

	private:
		CURL* curl;
		std::stringstream buffer;

		static size_t curlCallback(char* buffer, size_t size, size_t nmemb, Webclient* data);
};

#endif
