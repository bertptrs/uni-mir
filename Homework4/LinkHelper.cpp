#include "LinkHelper.hpp"
#include <iostream>

std::string LinkHelper::getProtocol(const std::string& url)
{
	size_t pos = url.find("://");
	if (pos == std::string::npos) {
		return "";
	} else {
		return url.substr(0, pos) + "://";
	}
}

std::string LinkHelper::getDomain(const std::string& url)
{
	const auto protocol = getProtocol(url);
	size_t endSlash = url.find('/', protocol.size());

	if (endSlash == std::string::npos) {
		// No terminating slash, return entire string minus protocol.
		return url.substr(protocol.size());
	} else {
		return url.substr(protocol.size(), endSlash - protocol.size());
	}
}

std::string LinkHelper::getDirectory(const std::string& url)
{
	const auto protocol = getProtocol(url);
	size_t pos = url.rfind('/');
	if (pos == std::string::npos || pos < protocol.size()) {
		return url + "/";
	}

	return url.substr(0, pos + 1);
}

std::string LinkHelper::relativize(const std::string& url, const std::string& base)
{
	size_t pos;
	if ((pos = url.find('#')) != std::string::npos) {
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
