#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <queue>
#include "Scraper.hpp"

class Crawler
{
	public:
		void startCrawl(const std::string& startingPoint);

	private:
		typedef std::pair<unsigned int, std::string> TodoEntry;

		Scraper scraper;

		std::unordered_map<std::string, unsigned int> domainVisits;
		std::unordered_set<std::string> queued;
		std::queue<TodoEntry> todo;

		void run();
		void queue(const std::string& url);

		static std::string hash(const std::string& data);
};

#endif
