#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <sstream>
#include <cassert>

#include "Searcher.hpp"

using namespace std;


// Two simple result combiners: either set conjunction or disjunction.
static vector<string> joinDisjunct(const vector<string>& a, const vector<string>& b)
{
	vector<string> result;

	set_intersection(a.begin(), a.end(),
			b.begin(), b.end(),
			back_inserter(result));

	return result;
}


static vector<string> joinConjunct(const vector<string>& a, const vector<string>& b)
{
	vector<string> result;

	set_union(a.begin(), a.end(),
			b.begin(), b.end(),
			back_inserter(result));

	return result;
}

// Utility function to read an inputstream into a vector, line by line.
// Also sorts them, because that is useful elsewhere.
static inline vector<string> readFile(istream& input)
{
	vector<string> result;
	string buffer;

	while (getline(input, buffer)) {
		result.push_back(buffer);
	}

	sort(result.begin(), result.end());

	return result;
}


Searcher::Searcher(const string& dataDir): dataDir(dataDir), helper(dataDir)
{
}

ifstream Searcher::getHandle(const string& filename) const
{
	stringstream fb;
	fb << dataDir << "/" << filename;

	return ifstream(fb.str());
}

Searcher::ResultList Searcher::search(istream& request) const
{
	const Json::Value requestData = loadJson(request);

	const auto combiner = requestData.get("conjuctive", Json::Value(false)).asBool()
		? joinConjunct
		: joinDisjunct;

	ResultList results, resultsBuffer;

	bool initialized = false;

	for (auto& term : requestData["terms"]) {
		const string& type = term["type"].asString();
		const string& word = term["word"].asString();

		// Handle different search types
		if (type == "keyword") {
			resultsBuffer = searchKeywords(word);
		} else if (type == "name") {
			resultsBuffer = searchName(word);
		} else if (type == "description") {
			resultsBuffer = searchDescription(word);
		} else {
			cerr << type << " is not a valid query type." << endl;
			exit(1);
		}

		if (!initialized) {
			results = resultsBuffer;
			initialized = true;
		} else {
			results = combiner(results, resultsBuffer);
		}
	}

	// TODO: sort the results

	return sortResults(results);
}


Searcher::ResultList Searcher::searchKeywords(const string& word) const
{
	try {
		auto handle = helper.readHandle(word, RepositoryType::KEYWORDS);

		return readFile(handle);
	} catch (RepositoryHelper::ItemNotAvailable) {
		return {};
	}
}

Searcher::ResultList Searcher::searchDescription(const string& word) const
{
	try {
		auto handle = helper.readHandle(word, RepositoryType::DESCRIPT);

		return readFile(handle);
	} catch (RepositoryHelper::ItemNotAvailable) {
		return {};
	}
}


Searcher::ResultList Searcher::searchName(const string& word) const
{
	ifstream packageHandle = getHandle("packages");

	ResultList results;
	string package;
	while (getline(packageHandle, package)) {
		if (package.find(word) != string::npos) {
			results.emplace_back(move(package));
		}
	}

	sort(results.begin(), results.end());

	return results;
}


Json::Value Searcher::loadJson(istream& input)
{
	Json::Value value;
	input >> value;

	return value;
}

Searcher::ResultList Searcher::sortResults(const ResultList& results) const
{
	ifstream pagerankHandle = getHandle("pagerank");

	vector<pair<double, string>> weightedResults;

	string packageName;
	double packageWeight;
	size_t pos = 0;

	// Combine weights with names, using that both lists are sorted.
	while (pagerankHandle >> packageName >> packageWeight) {
		for (; pos < results.size() && results[pos] < packageName; pos++);

		if (pos >= results.size()) {
			break;
		}

		if (packageName == results[pos]) {
			weightedResults.emplace_back(packageWeight, move(packageName));
			pos++;
		}
	}

	assert(weightedResults.size() == results.size() && "All results should have been found.");

	sort(weightedResults.begin(), weightedResults.end(), greater<pair<double, string>>());

	// Extract the package names from the weighted, sorted list.
	ResultList sortedResults;
	for (auto result : weightedResults) {
		sortedResults.emplace_back(move(result.second));
	}

	return sortedResults;
}
