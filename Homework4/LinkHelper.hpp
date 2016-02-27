#ifndef LINKHELPER_HPP
#define LINKHELPER_HPP

#include <string>
#include <unordered_set>

class LinkHelper
{
	public:
		static std::string getDomain(const std::string& url);
		static std::string getDirectory(const std::string& url);
		static std::string getProtocol(const std::string& url);

		static std::string relativize(const std::string& url, const std::string& base);

		static std::unordered_set<std::string> getURLWords(const std::string& url);
};

#endif
