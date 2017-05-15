#include "PackageCollection.hpp"
#include "Webclient.hpp"

using namespace std;

PackageCollection::PackageCollection()
{
	Webclient client;
	auto result = client.getURL("https://packagist.org/packages/list.json");
	for (const auto& entry : result["packageNames"])
	{
		packages.push_back(entry.asString());
	}
}


PackageCollection::PackageCollection(istream& input)
{
	string buffer;
	while (getline(input, buffer)) {
		packages.push_back(buffer);
	}
}

PackageCollection::const_iterator PackageCollection::begin() const
{
	return packages.begin();
}

PackageCollection::const_iterator PackageCollection::end() const
{
	return packages.end();
}

size_t PackageCollection::size() const
{
	return packages.size();
}
