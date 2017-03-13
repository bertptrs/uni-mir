#ifndef SCRAPER_HPP
#define SCRAPER_HPP

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include "Webclient.hpp"

class Scraper
{
	public:
		std::string load(const std::string& url);
		std::unordered_set<std::string> getWeblinks() const;
		std::unordered_set<std::string> getImagelinks() const;
		std::unordered_set<std::string> getWords() const;
		std::unordered_set<std::string> getTitleWords() const;
		std::unordered_map<std::string, std::unordered_set<std::string>> getImages() const;

		const std::string& getData() const;

	private:
		Webclient client;
		std::string url;
		std::string data;

		std::unordered_set<std::string> getLinksByTagName(const char* tag_name, const char* attribute) const;
		std::unordered_set<std::string> getWordsFromTag(const std::string& tag_name) const;
};

#endif
