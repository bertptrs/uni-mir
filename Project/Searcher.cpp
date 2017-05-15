#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <sstream>

#include "Searcher.hpp"
#include "PackageCollection.hpp"

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
		} else {
			cerr << type << " is not a valid query type." << endl;
			exit(1);
		}

		// TODO: actually use the query thingy

		if (!initialized) {
			results = resultsBuffer;
			initialized = true;
		} else {
			results = combiner(results, resultsBuffer);
		}
	}

	// TODO: sort the results

	return results;
}


Searcher::ResultList Searcher::searchKeywords(const string& word) const
{
	auto handle = helper.readHandle(word, RepositoryType::KEYWORDS);

	return readFile(handle);
}

Searcher::ResultList Searcher::searchName(const string& word) const
{
	stringstream sb;
	sb << dataDir << "/" << "packages";

	ifstream packageHandle(sb.str());

	ResultList results;
	for (auto& package : PackageCollection(packageHandle)) {
		if (package.find(word) != string::npos) {
			results.push_back(package);
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
