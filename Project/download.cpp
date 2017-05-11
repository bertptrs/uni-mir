#include <iostream>
#include <string>
#include <cstdlib>
#include "RepositoryHelper.hpp"
#include "PackageCollection.hpp"
#include "Webclient.hpp"

using namespace std;

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

	RepositoryHelper helper(dataDir);
	Webclient client;

	const string baseurl = "https://packagist.org/p/";
	const string suffix = ".json";

	for (auto package : PackageCollection()) {
		long mtime = helper.itemAge(package, RepositoryType::PACKAGES);
		try {
			auto data = client.getURL(baseurl + package + suffix, mtime);
			helper.writeHandle(package, RepositoryType::PACKAGES, true) << data;
			cout << "Downloaded " << package << endl;
		} catch (Webclient::CachedException) {
			cout << package << " up-to-date." << endl;
		} catch (Webclient::CrawlException) {
			cerr << "Error while downloading " << package << endl;
		}
	}
}
