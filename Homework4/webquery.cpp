#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <utility>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <queue>
#include "IndexHelper.hpp"

using namespace std;

const array<pair<int, string>, 3> INDEXWEIGHTS {{
	{16, "titleindex"},
	{4, "webindex"},
	{1, "pageindex"}
}};

string getQueryString()
{
	ifstream input("query.txt");
	string query;
	getline(input, query);

	return query;
}

map<string, int> getWeightedResults(const string& query)
{
	map<string, int> results;
	IndexHelper helper;

	// Add the base score from the index
	for (const auto index : INDEXWEIGHTS) {
		for (auto link : helper.getLinksFromIndex(query, index.second)) {
			results[link] += index.first;
		}
	}

	for (auto& entry : results) {
		entry.second += helper.getLinksToURL(entry.first);
	}

	return results;
}

vector<string> getResults(const string& query)
{
	stringstream tokens(query);
	queue<map<string, int>> totalResults;
	string token;
	while (tokens >> token) {
		if (token.empty()) {
			continue;
		}

		totalResults.push(getWeightedResults(token));
	}
	if (totalResults.empty()) {
		return {};
	}

	// Get the first keyword results
	auto weightedResults = totalResults.front();
	totalResults.pop();

	while (!totalResults.empty()) {
		auto newResults = totalResults.front();
		totalResults.pop();
		// Merge additional keyword results into them.
		auto currentIt = weightedResults.begin();
		auto newIt = newResults.begin();
		while (currentIt != weightedResults.end() && newIt != newResults.end()) {
			if (currentIt->first == newIt->first) {
				currentIt->second += newIt->second;
				currentIt++;
				newIt++;
			} else if (currentIt->first > newIt->first) {
				newIt++;
			} else {
				weightedResults.erase(currentIt++);
			}
		}
		weightedResults.erase(currentIt, weightedResults.end());
	}

	vector<pair<int, string>> orderedResults;
	for (auto entry : weightedResults) {
		orderedResults.emplace_back(entry.second, entry.first);
	}

	sort(orderedResults.begin(), orderedResults.end(), greater<pair<int, string>>());

	vector<string> results;
	for (auto result : orderedResults) {
		results.emplace_back(result.second);
	}

	return results;

}

int main()
{
	string query = getQueryString();
	for (auto result : getResults(query)) {
		cout << result << endl;
	}
	return 0;
}
