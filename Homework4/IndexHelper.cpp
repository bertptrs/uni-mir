#include <fstream>
#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include "IndexHelper.hpp"
#include "openssl/md5.h"

using namespace std;

set<string> IndexHelper::getLinksFromIndex(const string& word, const string& index) const
{
	set<string> links;
	string current;
	ifstream input("data/" + index + "/" + hash(word));

	while (getline(input, current)) {
		if (!current.empty()) {
			links.insert(current);
		}
	}

	return links;
}

int IndexHelper::getLinksToURL(const string& url) const
{
	auto filename = "data/linkindex/" + hash(url);
	ifstream input(filename);

	return count(istreambuf_iterator<char>(input),
			istreambuf_iterator<char>(), '\n');
}

bool IndexHelper::hasCrawledURL(const string& url) const
{
	struct stat buffer;
	auto filename = "data/repository/" + hash(url);

	return stat(filename.c_str(), &buffer) == 0;
}

string IndexHelper::hash(const string& data)
{
	unsigned char digest[MD5_DIGEST_LENGTH + 1];
	string hexDigest(2 * MD5_DIGEST_LENGTH, ' ');
	MD5((const unsigned char*) data.c_str(), data.size(), digest);
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
		sprintf(&hexDigest[2 * i], "%02x", digest[i]);
	}

	return hexDigest;
}

void IndexHelper::saveWordIndex(const string& url, const string& directory, const unordered_set<string>& words) const
{
	const auto prefix = "data/" + directory + "/";
	for (const auto& word : words) {
		ofstream output(prefix + hash(word), ios_base::out | ios_base::app);

		assert(output.good() && "File should be writable.");

		output << url << endl;
	}
}

void IndexHelper::savePageData(const string& url, const string& data) const
{
	auto filename = "data/repository/" + hash(url);
	ofstream output(filename, ios_base::out | ios_base::trunc);

	assert(output.good() && "File should be writeable.");

	output << data;
}

void IndexHelper::saveLinkToURL(const string& url, const string& referer) const
{
	ofstream output("data/linkindex/" + hash(url), ios_base::out | ios_base::app);

	assert(output.good() && "File should be writeable.");

	output << referer << endl;
}
