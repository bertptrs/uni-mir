#ifndef PACKAGECOLLECTION_H
#define PACKAGECOLLECTION_H

#include <vector>
#include <string>
#include <algorithm>

class PackageCollection
{
	public:
		typedef std::vector<std::string> PackageList;
		typedef PackageList::const_iterator const_iterator;

		PackageCollection();
		PackageCollection(std::istream& input);
		PackageCollection(const PackageCollection&) = default;

		const_iterator begin() const;
		const_iterator end() const;
		size_t size() const;


	private:
		PackageList packages;
};

#endif
