#ifndef REPOSITORYHELPER_HPP
#define REPOSITORYHELPER_HPP

#include <string>
#include <ctime>
#include <fstream>

enum class RepositoryType
{
	PACKAGES = 0,
	KEYWORDS = 1,
	REVDEPS  = 2,
	DEPENDS  = 3,
};

class RepositoryHelper
{
	public:
		struct ItemNotAvailable {};

		RepositoryHelper(const std::string& dir="data");
		RepositoryHelper(const RepositoryHelper&) = default;

		// Get the age of a particular index file, or -1 on non-existing.
		time_t itemAge(const std::string& item, const RepositoryType type) const;

		// File handle accesssors
		std::ifstream readHandle(const std::string& item, const RepositoryType type) const;

		std::ofstream writeHandle(const std::string& item, const RepositoryType type, bool truncate = false) const;


	private:
		const std::string basedir;

		static std::string hash(const std::string&);

		void verifyRepository(const RepositoryType type) const;
		std::string itemPath(const std::string& item, const RepositoryType type) const;

};

#endif
