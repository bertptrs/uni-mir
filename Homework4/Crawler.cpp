#include <iostream>
#include <cstdio>
#include <fstream>
#include <cassert>
#include <csignal>
#include "Crawler.hpp"
#include "LinkHelper.hpp"
#include "Webclient.hpp"

using namespace std;

static bool interrupted = false;
static bool registered = false;

Crawler::Crawler()
{
	registerHandler();
}

void Crawler::registerHandler()
{
	if (!registered) {
		registered = true;
		signal(SIGINT, handler);
	}
}

void Crawler::handler(int)
{
	cerr << "Received interrupt!" << endl;
	interrupted = true;
}

void Crawler::startCrawl(const string& startingPoint)
{
	const auto url = LinkHelper::relativize(startingPoint, "");
	if (LinkHelper::getProtocol(url) == "") {
		cerr << "Cannot start at " << url << endl;
		return;
	}

	queue(url);
	run();
}

void Crawler::run()
{
	int i = 0;
	while (!todo.empty() && !interrupted) {
		string entry = todo.front();
		todo.pop();
		queued.erase(entry);

		if (indexHelper.hasCrawledURL(entry)) {
			continue;
		}
		if (recentlyVisitedDomain(entry)) {
			queue(entry);
			continue;
		}
		if (!robotsHandler.isAllowed(entry)) {
			cerr << "Skipping " << entry << " due to robots.txt" << endl;
			continue;
		}
		cout << "Request " << ++i << ": " << entry << endl;
		cout << "Domains: " << domainVisits.size() << endl;
		cout << "Queue size: " << todo.size() << endl << endl;

		try {
			string effective = scraper.load(entry);
			domainVisits[LinkHelper::getDomain(entry)] = Clock::now();

			// update all indexes
			indexHelper.saveWordIndex(effective, "pageindex", scraper.getWords());
			indexHelper.saveWordIndex(effective, "titleindex", scraper.getTitleWords());
			indexHelper.saveWordIndex(effective, "webindex", LinkHelper::getURLWords(effective));
			indexHelper.savePageData(effective, scraper.getData());

			// Queue all links in the page.
			for (auto url : scraper.getWeblinks()) {
				queue(url);
				indexHelper.saveLinkToURL(url, entry);
			}
		} catch (Webclient::CrawlException ex) {
			cerr << "Failed to load url " << entry << endl;
		}
	}
}

void Crawler::queue(const string& url)
{
	if (queued.count(url) || indexHelper.hasCrawledURL(url)) {
		// Already visited this one
		return;
	}

	queued.insert(url);

	todo.push(url);
}

bool Crawler::recentlyVisitedDomain(const string& url) const
{
	try {
		return Clock::now() - domainVisits.at(LinkHelper::getDomain(url)) < DOMAIN_REQUEST_INTERVAL;
	} catch (out_of_range) {
		return false;
	}
}
