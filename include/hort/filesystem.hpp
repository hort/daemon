#ifndef HORT_FILESYSTEM_HPP_
#define HORT_FILESYSTEM_HPP_

#include "hort/string.hpp"

namespace hort::filesystem {

/// \brief Returns true if file exists.
[[nodiscard]] inline bool exists(const std::string& path);

/// \brief Returns true if file exists.
template <typename... Args>
[[nodiscard]] inline bool exists(const Args&... args) {
  return exists("/"_join(args...));
}

/// \brief Create directories to path.
inline int mkpath(std::string path);

/// \brief Create directories to path.
template <typename... Args>
inline bool mkpath(const Args&... args) {
  return mkpath("/"_join(args...));
}

/// \brief Save bin to filepath/filename. If file already exists overwrites it.
inline int write(const std::string& source,
                 const std::string& filepath,
                 const std::string& filename);

/// \brief Sanitize filepath replacing all occurences of '/' with '-'.
[[nodiscard]] inline std::string sanitize(const std::string& str);

/// \brief Returns the base nome of the path.
[[nodiscard]] inline std::string base_name(const std::string& path);

} // namespace hort::filesystem

#endif /* HORT_FILESYSTEM_HPP_ */
