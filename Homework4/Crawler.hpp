#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <queue>
#include <chrono>
#include "Scraper.hpp"

class Crawler
{
	public:
		void startCrawl(const std::string& startingPoint);

	private:
		typedef std::chrono::steady_clock Clock;

		static constexpr auto DOMAIN_REQUEST_INTERVAL = std::chrono::seconds(2);

		Scraper scraper;

		std::unordered_map<std::string, Clock::time_point> domainVisits;
		std::unordered_set<std::string> visited;
		std::queue<std::string> todo;

		void run();
		void queue(const std::string& url);
		void saveData(const std::string& url) const;
		bool recentlyVisitedDomain(const std::string& url) const;

		static std::string hash(const std::string& data);
};

#endif
