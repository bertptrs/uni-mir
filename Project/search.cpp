#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cassert>

#include "Searcher.hpp"

using namespace std;

void usage(const char* name)
{
	cerr << "Usage: " << name << " searchfile [data dir]" << endl;
	exit(1);
}

int main(const int argc, const char* argv[])
{
	const char* dataDir;
	switch (argc) {
		case 2:
			dataDir = "data";
			break;

		case 3:
			dataDir = argv[2];
			break;

		default:
			usage(argv[0]);
	}

	Searcher s(dataDir);
	ifstream searchFile(argv[1]);

	assert(searchFile.good() && "Input should be readable");

	for (auto package : s.search(searchFile)) {
		cout << package << "\n";
	}

	return 0;

}
