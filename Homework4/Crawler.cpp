#include <iostream>
#include <cstdio>
#include <fstream>
#include <cassert>
#include "Crawler.hpp"
#include "LinkHelper.hpp"
#include "Webclient.hpp"
#include "openssl/md5.h"

void Crawler::startCrawl(const std::string& startingPoint)
{
	const auto url = LinkHelper::relativize(startingPoint, "");
	if (LinkHelper::getProtocol(url) == "") {
		std::cerr << "Cannot start at " << url << std::endl;
		return;
	}
	queue(url);
	run();
}

void Crawler::run()
{
	int i = 0;
	while (!todo.empty()) {
		TodoEntry entry = todo.front();
		todo.pop();
		if (visited.count(entry.second)) {
			continue;
		}
		std::cout << "Request " << ++i << ": " << entry.second << std::endl;
		std::cout << "Domains: " << domainVisits.size() << std::endl;
		std::cout << "Queue size: " << todo.size() << std::endl << std::endl;

		try {
			std::string effective = scraper.load(entry.second);
			visited.insert(effective);

			saveData(effective);

			for (auto url : scraper.getWeblinks()) {
				queue(url);
			}
		} catch (Webclient::CrawlException ex) {
			std::cerr << "Failed to load url " << entry.second << std::endl;
		}
	}
}

void Crawler::queue(const std::string& url)
{
	if (visited.count(url)) {
		// Already visited this one
		return;
	}

	TodoEntry entry = make_pair(domainVisits[LinkHelper::getDomain(url)]++, url);
	todo.push(entry);
}

std::string Crawler::hash(const std::string& data)
{
	unsigned char digest[MD5_DIGEST_LENGTH + 1];
	std::string hexDigest(2 * MD5_DIGEST_LENGTH, ' ');
	MD5((const unsigned char*) data.c_str(), data.size(), digest);
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
		sprintf(&hexDigest[2 * i], "%02x", digest[i]);
	}

	return hexDigest;
}

void Crawler::saveData(const std::string& url) const
{
	auto filename = "data/repository/" + hash(url);
	std::ofstream output(filename, std::ios_base::out | std::ios_base::trunc);

	assert(output.good() && "File should be writeable.");

	output << scraper.getData();
}
