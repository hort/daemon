#ifndef FILESYSTEM_HPP_
#define FILESYSTEM_HPP_

#include <string>

namespace hort::filesystem
{

/// \brief Returns true if file exists.
bool exists(const std::string &name);

/// \brief Create directory at path. If parent directories don't exist, create them.
int mkpath(std::string path);

/// \brief Save bin to filepath/filename. Does not check if file already exists.
int save(const std::string &bin, const std::string &filepath, const std::string &filename);

} // namespace hort::filesystem

#endif /* FILESYSTEM_HPP_ */
