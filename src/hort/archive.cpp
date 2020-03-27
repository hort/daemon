#include "hort/archive.hpp"

#include <cstring>

namespace hort {

bool archive::add(const hort::string& filepath, const std::string& source) {
  std::lock_guard<std::mutex> lock{mutex};
  if (!zp) return false;

  zip_source* zs = zip_source_buffer(zp, source.c_str(), source.size(), 1);
  int index      = zip_file_add(zp, filepath.c_str(), zs, ZIP_FL_OVERWRITE);

  if (index != -1) {
    files.emplace_back(stat{filepath, files.size(), source.size()});
  } else {
    zip_source_free(zs);
    return false;
  }

  return true;
}

void archive::read() {
  std::lock_guard<std::mutex> lock{mutex};
  if (!zp) return;

  struct zip_stat zs;

  for (std::size_t i = 0; i < zip_get_num_entries(zp, 0); i++) {
    if (zip_stat_index(zp, i, 0, &zs) == 0) {
      files.push_back(stat{zs.name, i, zs.size});
    }
  }
}

} // namespace hort
