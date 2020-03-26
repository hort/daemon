#include "hort/archive.hpp"

#include <cstring>

namespace hort {

bool archive::open(const std::string& filepath) {
  std::lock_guard<std::mutex> lock{mutex};
  if (zp) {
    return false;
  }
  zp = zip_open(filepath.c_str(), ZIP_CREATE, nullptr);
  return zp != nullptr;
}

void archive::close() {
  std::lock_guard<std::mutex> lock{mutex};
  if (zp) {
    zip_close(zp);
    zp = nullptr;
  }
}

bool archive::add(const hort::string& filepath, const std::string& source) {
  std::lock_guard<std::mutex> lock{mutex};
  if (!zp) {
    return false;
  }

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

bool archive::remove(const std::string& filepath) {
  auto index =
      files.index([&filepath](const stat& s) { return s.name == filepath; });
  return zip_delete(zp, index) != -1;
}

void archive::read() {
  std::lock_guard<std::mutex> lock{mutex};
  if (!zp) {
    return;
  }

  struct zip_stat zs;

  for (std::size_t i = 0; i < zip_get_num_entries(zp, 0); i++) {
    if (zip_stat_index(zp, i, 0, &zs) == 0) {
      files.push_back(stat{zs.name, i, zs.size});
    }
  }
}

} // namespace hort
