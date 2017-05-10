#include <iostream>
#include <string>
#include "RepositoryHelper.hpp"
#include "PackageCollection.hpp"
#include "Webclient.hpp"

using namespace std;

int main()
{
	RepositoryHelper helper;
	Webclient client;
	const string baseurl = "https://packagist.org/p/";
	const string suffix = ".json";
	for (auto package : PackageCollection()) {
		long mtime = helper.itemAge(package, RepositoryType::PACKAGES);
		try {
			auto data = client.getURL(baseurl + package + suffix, mtime);
			helper.storeData(package, data, RepositoryType::PACKAGES);
			cout << "Downloaded " << package << endl;
		} catch (Webclient::CachedException) {
			cout << package << " up-to-date." << endl;
		} catch (Webclient::CrawlException) {
			cerr << "Error while downloading " << package << endl;
		}
	}
}
