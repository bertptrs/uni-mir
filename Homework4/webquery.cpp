#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <utility>
#include <map>
#include <vector>
#include <algorithm>
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

	// Filter out links that have not been crawled yet.
	for (auto it = results.begin(); it != results.end(); ) {
		if (!helper.hasCrawledURL(it->first)) {
			cout << "Erased " << it->first << endl;
			results.erase(it++);
		} else {
			++it;
		}
	}

	return results;
}

vector<string> getResults(string query)
{
	vector<pair<int, string>> orderedResults;
	for (auto entry : getWeightedResults(query)) {
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
	string query = "liacs";
	for (auto result : getResults(query)) {
		cout << result << endl;
	}
	return 0;
}