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
#include <cstring>
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

	return results;
}

vector<string> getResults(const string& query)
{
	IndexHelper helper;
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

	for (auto& entry : weightedResults) {
		entry.second += helper.getLinksToURL(entry.first);
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

vector<string> getImageResults(const string& query)
{
    IndexHelper helper;
    stringstream tokenStream(query);

    string token;
    tokenStream >> token;

    auto results = helper.getLinksFromIndex(token, "imageindex");

    while (tokenStream >> token) {
        cerr << "'" << token << "'" << endl;
        auto additionalResults = helper.getLinksFromIndex(token, "imageindex");
        cerr << additionalResults.size() << endl;
        decltype(additionalResults) resultsCopy;

        set_intersection(results.begin(), results.end(),
                additionalResults.begin(), additionalResults.end(),
                insert_iterator<set<string>>(resultsCopy, resultsCopy.end()));

        results = resultsCopy;
    }

    return vector<string>(results.begin(), results.end());
}

int main(int argc, char** argv)
{
	string query = getQueryString();
    if (argc == 1 || strcmp(argv[1], "--image")) {
        // Do text search
        for (auto result : getResults(query)) {
            cout << result << endl;
        }
    } else {
        // Do image search
        for (auto result : getImageResults(query)) {
            cout << result << endl;
        }
    }
	return 0;
}
