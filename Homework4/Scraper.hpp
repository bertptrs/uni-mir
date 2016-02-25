#ifndef SCRAPER_HPP
#define SCRAPER_HPP

#include <string>
#include <vector>
#include <set>
#include "Webclient.hpp"

class Scraper
{
	public:
		std::string load(const std::string& url);
		std::vector<std::string> getWeblinks() const;
		std::vector<std::string> getImagelinks() const;
		std::vector<std::string> getWords();

		const std::string& getData() const;

	private:
		Webclient client;
		std::string url;
		std::string data;

		std::vector<std::string> getLinksByTagName(const char* tag_name, const char* attribute) const;
};

#endif
