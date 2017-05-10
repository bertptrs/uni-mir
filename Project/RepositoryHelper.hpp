#ifndef REPOSITORYHELPER_HPP
#define REPOSITORYHELPER_HPP

#include <string>
#include <ctime>
#include <fstream>

enum class RepositoryType
{
	PACKAGES = 0,
};

class RepositoryHelper
{
	public:
		RepositoryHelper(const std::string& dir="data");
		RepositoryHelper(const RepositoryHelper&) = default;

		time_t itemAge(const std::string& item, const RepositoryType type) const;
		std::string itemPath(const std::string& item, const RepositoryType type) const;

		template<class T>
			void storeData(const std::string& item, const T& data, const RepositoryType type) {
				std::fstream f(itemPath(item, type), std::ios_base::out | std::ios_base::trunc);
				f << data;
			}

	private:
		const std::string basedir;

		static std::string hash(const std::string&);

		void verifyRepository(const RepositoryType type) const;

};

#endif
