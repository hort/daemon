#ifndef ZIP_HPP_
#define ZIP_HPP_

#include <map>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <zip.h>

namespace hort
{

/// \brief Thin wrapper around libzip archive.
class Archive
{

	zip *archive;
	std::vector<char*> allocated;

public:
	Archive();
	Archive(const std::string &filepath);
	~Archive();

	bool open(const std::string &filepath);
	void close();

	/// \bierf Add binary data to archive.
	/// \return index of the newly added file -1 on error, -2 if the archive hasn't been opened.
	int add(const std::string &filepath, const std::string &bin);

	/// \brief Add files to archive in bulk.
	/// \return 0 on success, -1 on error, -2 if the archive hasn't been opened.
	int add(const std::map<std::string, std::string> &files);

};

} // namespace hort

#endif /* ZIP_HPP_ */
