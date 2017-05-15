#include <iostream>
#include <set>
#include <algorithm>
#include <fstream>
#include "json/json.h"
#include "PackageCollection.hpp"
#include "RepositoryHelper.hpp"

using namespace std;

class Indexer {
	public:
		Indexer(const string& dataDir);

		bool index(const string& packageName) const;

	private:
		RepositoryHelper helper;

		Json::Value loadPackageData(const string& packageName) const;

		template<class T>
			void addToIndices(const string& word, const T& indices, RepositoryType type) const {
				for (auto& index : indices) {
					ofstream handle = helper.writeHandle(index, type);
					handle << word << "\n";
				}
			}

		template<class T>
			void addToIndex(const T& items, const string& index, RepositoryType type) const {
				if (items.empty()) {
					return;
				}

				ofstream handle = helper.writeHandle(index, type);
				for (auto& item : items) {
					handle << item << "\n";
				}
			}
};

Indexer::Indexer(const string& dataDir) : helper(dataDir)
{
}

bool Indexer::index(const string& packageName) const
{
	try {
		const auto data = loadPackageData(packageName);

		set<string> keywordList, depList;

		// Iterate over package versions
		for (auto& version : data["packages"][packageName]) {
			auto& keywords = version["keywords"];

			transform(keywords.begin(), keywords.end(),
					insert_iterator<set<string>>(keywordList, keywordList.begin()),
					[](const Json::Value& x) { return x.asString(); });

			const auto dependencies = version["require"].getMemberNames();

			depList.insert(dependencies.begin(), dependencies.end());
		}

		addToIndices(packageName, keywordList, RepositoryType::KEYWORDS);
		addToIndices(packageName, depList, RepositoryType::REVDEPS);
		addToIndex(depList, packageName, RepositoryType::DEPENDS);

		return true;
	} catch (Json::LogicError ex) {
		cerr << "Error while parsing " << packageName << ":\n"
			<< ex.what() << endl;
		return false;
	} catch (RepositoryHelper::ItemNotAvailable) {
		return false;
	}
}

Json::Value Indexer::loadPackageData(const string& packageName) const
{
	auto dataStream = helper.readHandle(packageName, RepositoryType::PACKAGES);
	Json::Value data;

	dataStream >> data;

	return data;
}

void showHelp(const char* argv[])
{
	cerr << "Usage: " << argv[0] << " [data dir]" << endl;
	exit(1);
}

int main(int argc, const char* argv[])
{
	// Parse arguments
	const char* dataDir;
	switch (argc) {
		case 1:
			dataDir = "data";
			break;

		case 2:
			dataDir = argv[1];
			break;

		default:
			showHelp(argv);
	}

	Indexer indexer(dataDir);
	ofstream packages(string(dataDir) + "/packages");

	for (auto package : PackageCollection()) {
		if (indexer.index(package)) {
			packages << package << "\n";
		}
	}

	return 0;

}
