#ifndef HORT_FILESYSTEM_HPP_
#define HORT_FILESYSTEM_HPP_

#include "hort/string.hpp"

namespace hort::filesystem {

/// \brief Returns true if file exists.
[[nodiscard]] bool exists(const string& path);

/// \brief Returns true if file exists.
template <typename... Args>
[[nodiscard]] inline bool exists(const Args&... args) {
  return exists(join("/", args...));
}

/// \brief Create directories to path.
bool mkpath(string path);

/// \brief Create directories to path.
template <typename... Args>
inline bool mkpath(const Args&... args) {
  return mkpath(join("/", args...));
}

/// \brief Save bin to filepath/filename. If file already exists overwrites it.
bool write(const string& source,
           const string& filepath,
           const string& filename);

/// \brief Sanitize filepath by removing all '/'
[[nodiscard]] string sanitize(const string& str);

/// \brief Returns the base nome of the path.
[[nodiscard]] string base_name(const string& path);

} // namespace hort::filesystem

#endif /* HORT_FILESYSTEM_HPP_ */
