#ifndef ROBOTSHANDLER_HPP
#define ROBOTSHANDLER_HPP

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "Webclient.hpp"

class RobotsHandler
{
	public:
		bool isAllowed(const std::string& url);

	private:
		Webclient client;
		std::unordered_map<std::string, std::unordered_set<std::string>> robotsTxtCache;

		void loadRobots(const std::string& domain);
};

#endif
