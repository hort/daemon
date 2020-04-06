#ifndef HORT_ARCHIVE_HPP_
#define HORT_ARCHIVE_HPP_

#include <map>
#include <mutex>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#include <zip.h>

#include "hort/string.hpp"
#include "hort/vector.hpp"

namespace hort {

/// \brief Wrapper around libzip archive. Thread safe.
/// TODO: create directories when adding files
class archive {
private:
  std::mutex mutex;

  zip* zp = nullptr;

  struct stat {
    std::string name;
    std::size_t index;
    std::size_t size;
  };

  /// \brief List of files in archive.
  vector<stat> files;

public:
  explicit archive() : archive{nullptr} {}
  explicit archive(const std::string& filepath) { open(filepath); }
  ~archive() { close(); };

  /// \return Returns true if succesful, otherwise false.
  bool open(const std::string& filepath);

  /// \brief Write changes to archive.
  void close();

  /// \brief Add binary data to archive.
  /// \return Returns true if succesful, otherwise false.
  bool add(const hort::string& filepath, const std::string& source);

  /// \brief Delete `filepath`.
  /// \return Returns true if succesful, otherwise false.
  bool remove(const std::string& filepath);

  /// \brief Read archive stats.
  void read();
};

} // namespace hort

#endif /* ZIP_ARCHIVE_HPP_ */
