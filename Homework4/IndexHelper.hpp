#ifndef INDEXHELPER_HPP
#define INDEXHELPER_HPP

#include <string>
#include <unordered_set>

class IndexHelper
{
	public:
		void saveLinkToURL(const std::string& url, const std::string& refererer) const;
		void saveWordIndex(const std::string& url, const std::string& directory, const std::unordered_set<std::string>& words) const;
		void savePageData(const std::string& url, const std::string& data) const;

	private:
		static std::string hash(const std::string& data);
};

#endif
