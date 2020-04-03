#include "hort/filesystem.hpp"
#include "hort/regexp.hpp"

#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

namespace hort::filesystem {

bool exists(const std::string& path) {
  struct stat buffer;
  return (stat(name.data(), &buffer) == 0);
}

int mkpath(std::string path) {
  if (exists(path)) return 0;

  std::string::size_type pos = 0;
  std::string dir;
  int mdret;

  if (path[path.size() - 1] != '/') {
    path += '/';
  }

  while ((pos = path.find_first_of('/', pos)) != std::string::npos) {
    dir = path.substr(0, pos++);
    if (dir.size() == 0) continue;
    if ((mdret = mkdir(dir.c_str(), 0775)) && errno != EEXIST) {
      return mdret;
    }
  }

  return mdret;
}

int write(const std::string& source,
          const std::string& filepath,
          const std::string& filename) {
  mkpath(filepath);
  std::ofstream file;
  file.open(filepath + "/" + filename);
  if (file.fail())
    return 1;
  file << source;
  file.close();
  return 0;
}

void sanitize(std::string& str) {
  hort::regexp::replaceall("/", "-", str);
}

std::string base_name(const std::string &path) {
  auto pos = path.find_last_of("/");
  if (pos != std::string::npos) {
    return path.substr(pos, path.length());
  } else {
    return path;
  }
}

} // namespace hort::filesystem
