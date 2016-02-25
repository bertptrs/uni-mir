#include <iostream>
#include <string>
#include "Crawler.hpp"

using namespace std;

int main(int argc, char** argv)
{
	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " URL" << endl;
		return 1;
	}
	Crawler crawler;
	crawler.startCrawl(argv[1]);
	return 0;
}
