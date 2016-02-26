#include <iostream>
#include <cstdio>
#include <fstream>
#include <cassert>
#include "Crawler.hpp"
#include "LinkHelper.hpp"
#include "Webclient.hpp"
#include "openssl/md5.h"

using namespace std;

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
	while (!todo.empty()) {
		string entry = todo.front();
		todo.pop();
		if (visited.count(entry)) {
			continue;
		}
		if (recentlyVisitedDomain(entry)) {
			queue(entry);
			continue;
		}
		cout << "Request " << ++i << ": " << entry << endl;
		cout << "Domains: " << domainVisits.size() << endl;
		cout << "Queue size: " << todo.size() << endl << endl;

		try {
			domainVisits[LinkHelper::getDomain(entry)] = Clock::now();
			string effective = scraper.load(entry);
			visited.insert(effective);

			saveData(effective);

			for (auto url : scraper.getWeblinks()) {
				queue(url);
				saveLinkToURL(url, entry);
			}
		} catch (Webclient::CrawlException ex) {
			cerr << "Failed to load url " << entry << endl;
		}
	}
}

void Crawler::queue(const string& url)
{
	if (visited.count(url)) {
		// Already visited this one
		return;
	}

	todo.push(url);
}

string Crawler::hash(const string& data)
{
	unsigned char digest[MD5_DIGEST_LENGTH + 1];
	string hexDigest(2 * MD5_DIGEST_LENGTH, ' ');
	MD5((const unsigned char*) data.c_str(), data.size(), digest);
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
		sprintf(&hexDigest[2 * i], "%02x", digest[i]);
	}

	return hexDigest;
}

void Crawler::saveData(const string& url) const
{
	auto filename = "data/repository/" + hash(url);
	ofstream output(filename, ios_base::out | ios_base::trunc);

	assert(output.good() && "File should be writeable.");

	output << scraper.getData();
}

void Crawler::saveLinkToURL(const string& url, const string& referer) const
{
	auto filename = "data/linkindex/" + hash(url);
	ofstream output(filename, ios_base::out | ios_base::app);

	assert(output.good() && "File should be writable.");

	output << referer << endl;
}

bool Crawler::recentlyVisitedDomain(const string& url) const
{
	try {
		return Clock::now() - domainVisits.at(LinkHelper::getDomain(url)) < DOMAIN_REQUEST_INTERVAL;
	} catch (out_of_range) {
		return false;
	}
}
