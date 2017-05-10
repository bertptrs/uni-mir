#include "Webclient.hpp"

using namespace std;
using namespace Json;

Webclient::Webclient() :
	curl(curl_easy_init())
{
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Webclient::curlCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 2);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
	curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "MIRBot/1.0 - A robot made for a course at Leiden University - Abuse: l.j.peters@umail.leidenuniv.nl");
}

Webclient::~Webclient()
{
	// Cleanup CURL
	curl_easy_cleanup(curl);
}

Value Webclient::getURL(const string& url)
{
	// Set the link
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	// Clear the current buffer
	buffer.str("");

	CURLcode res = curl_easy_perform(curl);
	long http_code;

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

	if (res != 0 || http_code != 200) {
		throw CrawlException();
	}

	Value value;
	buffer >> value;

	return value;
}

size_t Webclient::curlCallback(char* buffer, size_t size, size_t nmemb, Webclient* client)
{
	const size_t real_size = size * nmemb;

	client->buffer.write(buffer, real_size);

	return real_size;
}
