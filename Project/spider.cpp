#include <iostream>
#include "PackageCollection.hpp"

using namespace std;

int main()
{
	cout << "Hello world!" << endl;

	PackageCollection c;
	for (auto p : c) {
		cout << p << endl;
	}

	return 0;
}
