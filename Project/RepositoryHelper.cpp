#include "RepositoryHelper.hpp"
#include <sys/stat.h>
#include <sstream>
#include <cassert>
#include <openssl/md5.h>
#include <iostream>

using namespace std;

RepositoryHelper::RepositoryHelper(const string& dir) :
	basedir(dir)
{
}

time_t RepositoryHelper::itemAge(const std::string& item, const RepositoryType type) const
{
	struct stat sb;
	if (stat(itemPath(item, type).c_str(), &sb) == 0) {
		return sb.st_mtime;
	}

	return -1;
}

std::string RepositoryHelper::itemPath(const string& item, const RepositoryType type) const
{
	verifyRepository(type);
	stringstream pb;
	pb << basedir << "/" << (int) type << "/" << hash(item);
	return pb.str();
}

void RepositoryHelper::verifyRepository(const RepositoryType type) const
{
	stringstream pb;
	pb << basedir << "/" << ((int) type);
	const string path = pb.str();

	struct stat sb;
	if (stat(path.c_str(), &sb) != 0 || !S_ISDIR(sb.st_mode))
	{
		// Attempt to create the directory
		assert(mkdir(path.c_str(), 0777) == 0 && "Directory should be created.");
	}
}

ifstream RepositoryHelper::readHandle(const string& item, const RepositoryType type) const
{
	auto s = ifstream(itemPath(item, type));
	if (!s.good()) {
		throw ItemNotAvailable();
	}

	return s;
}

ofstream RepositoryHelper::writeHandle(const std::string& item, const RepositoryType type, bool truncate) const
{
	auto flags = truncate
		? (ios_base::out | ios_base::trunc)
		: (ios_base::out | ios_base::app);

	return ofstream(itemPath(item, type), flags);
}

string RepositoryHelper::hash(const string& data)
{
	unsigned char digest[MD5_DIGEST_LENGTH + 1];
	string hexDigest(2 * MD5_DIGEST_LENGTH, ' ');
	MD5((const unsigned char*) data.c_str(), data.size(), digest);
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
		sprintf(&hexDigest[2 * i], "%02x", digest[i]);
	}

	return hexDigest;
}
