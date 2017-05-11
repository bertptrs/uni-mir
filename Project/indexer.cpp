#include <iostream>
#include "json/json.h"
#include "PackageCollection.hpp"
#include "RepositoryHelper.hpp"

using namespace std;

void showHelp(const char* argv[])
{
	cerr << "Usage: " << argv[0] << " [data dir]" << endl;
	exit(1);
}

void doIndex(const string& package, const RepositoryHelper& helper)
{
	Json::Value data;
	try {
		auto dataStream = helper.readHandle(package, RepositoryType::PACKAGES);
		dataStream >> data;
	} catch (RepositoryHelper::ItemNotAvailable) {
		cerr << "Missing repository data for " << package << endl;
		return;
	}
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

	RepositoryHelper helper(dataDir);

	for (auto package : PackageCollection()) {
		doIndex(package, helper);
	}

}
