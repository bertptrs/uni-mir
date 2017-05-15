#ifndef SEARCHER_HPP
#define SEARCHER_HPP

#include <string>
#include <vector>
#include <iostream>

#include "json/json.h"

#include "RepositoryHelper.hpp"

class Searcher
{
	public:
		typedef std::vector<std::string> ResultList;

		Searcher(const std::string& dataDir);

		ResultList search(std::istream& searchData) const;

	private:
		const std::string dataDir;
		const RepositoryHelper helper;

		static Json::Value loadJson(std::istream& data);


		ResultList searchName(const std::string&) const;
		ResultList searchKeywords(const std::string&) const;
};

#endif
