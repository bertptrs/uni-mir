#include <sstream>
#include <iostream>
#include <cstring>
#include <limits>
#include "RobotsHandler.hpp"
#include "LinkHelper.hpp"

using namespace std;


bool RobotsHandler::isAllowed(const string& url)
{
	const auto domain = LinkHelper::getDomain(url);
	if (robotsTxtCache.count(domain) == 0) {
		loadRobots(domain);
	}

	const auto protocol = LinkHelper::getProtocol(url);

	for (const auto& prefix : robotsTxtCache[domain])
	{
		const string needle = protocol + domain + prefix;
		if (url.find(needle) == 0) {
			return false;
		}
	}

	return true;
}

void RobotsHandler::loadRobots(const string& domain)
{
	stringstream contents;
	try {
		auto url = "http://" + domain + "/robots.txt";
		contents << client.getURL(url);
	} catch (Webclient::CrawlException) {
		// Probably non-existent
	}

	while (contents) {
		string word ;
		contents >> word;

		if (strcasecmp(word.c_str(), "Disallow:") == 0) {
			contents >> word;
			robotsTxtCache[domain].insert(word);
		}

		contents.ignore(numeric_limits<std::streamsize>::max(), '\n');
	}

}
