#include "Webclient.hpp"

Webclient::Webclient() :
	curl(curl_easy_init())
{
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Webclient::curlCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 2);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
	curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "GoogleLiteBot");
}

Webclient::~Webclient()
{
	// Cleanup CURL
	curl_easy_cleanup(curl);
}

std::string Webclient::getURL(std::string& url)
{
	// Set the link
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	// Clear the current buffer
	buffer.str("");

	CURLcode res = curl_easy_perform(curl);

	if (res != 0) {
		CrawlException ex;
		throw ex;
	}

	char* effectiveURL;
	curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effectiveURL);
	url = effectiveURL;

	return buffer.str();
}

size_t Webclient::curlCallback(char* buffer, size_t size, size_t nmemb, Webclient* client)
{
	const size_t real_size = size * nmemb;

	client->buffer.write(buffer, real_size);

	return real_size;
}
