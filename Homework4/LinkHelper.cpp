#include "LinkHelper.hpp"
#include <cctype>
#include <sstream>

using namespace std;

string LinkHelper::getProtocol(const string& url)
{
	size_t pos = url.find("://");
	if (pos == string::npos) {
		return "";
	} else {
		return url.substr(0, pos) + "://";
	}
}

string LinkHelper::getDomain(const string& url)
{
	const auto protocol = getProtocol(url);
	size_t endSlash = url.find('/', protocol.size());

	if (endSlash == string::npos) {
		// No terminating slash, return entire string minus protocol.
		return url.substr(protocol.size());
	} else {
		return url.substr(protocol.size(), endSlash - protocol.size());
	}
}

string LinkHelper::getDirectory(const string& url)
{
	const auto protocol = getProtocol(url);
	size_t pos = url.rfind('/');
	if (pos == string::npos || pos < protocol.size()) {
		return url + "/";
	}

	return url.substr(0, pos + 1);
}

string LinkHelper::relativize(const string& url, const string& base)
{
	size_t pos;
	if ((pos = url.find('#')) != string::npos) {
		// Strip the anchor and try again.
		return relativize(url.substr(0, pos), base);
	}

	if (url.empty()) {
		// Resolve to self.
		return base;
	}

	if (getProtocol(url) != "") {
		// Fully qualified URL
		if (url == getProtocol(url) + getDomain(url)) {
			// Domain without trailing slash. Append slash
			return url + "/";
		} else {
			// Assume valid URL, return it.
			return url;
		}
	}

	if (url[0] == '/') {
		if (url.size() > 1 && url[1] == '/') {
			// Protocol relative URL. Prepend protocol and try again.
			return relativize(getProtocol(base) + url.substr(2), base);
		}

		// Domain relative URL
		return getProtocol(base) + getDomain(base) + url;
	}

	// Document relative URL
	return getDirectory(base) + url;
}

unordered_set<string> LinkHelper::getURLWords(const string& url)
{
	unordered_set<string> words;
	string word;
	stringstream currentWord;
	for (char c : url)
	{
		if (isalnum(c)) {
			currentWord << tolower(c);
		} else {
			currentWord >> word;
			words.insert(word);
		}
	}

	currentWord >> word;
	if (!word.empty()) {
		words.insert(word);
	}

	return words;
}
