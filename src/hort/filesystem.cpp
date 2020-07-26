#include "hort/filesystem.hpp"

#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

namespace hort::filesystem {

bool exists(const string& path) {
  struct stat buffer;
  return (stat(path.data(), &buffer) == 0);
}

bool mkpath(string path) {
  if (exists(path))
    return 0;

  string::size_type pos = 0;
  string dir;
  int mdret;

  if (path[path.size() - 1] != '/') {
    path += '/';
  }

  while ((pos = path.find_first_of('/', pos)) != string::npos) {
    dir = path.substr(0, pos++);
    if (dir.size() == 0)
      continue;
    if ((mdret = mkdir(dir.c_str(), 0775)) && errno != EEXIST) {
      return mdret;
    }
  }

  return mdret;
}

bool write(const string& source,
           const string& filepath,
           const string& filename) {
  mkpath(filepath);
  std::ofstream file;
  file.open(filepath + "/" + filename);
  if (file.fail())
    return 1;
  file << source;
  file.close();
  return 0;
}

string sanitize(const string &str) {
  return str.replace("/", "");
}

string base_name(const string& path) {
  auto pos = path.find_last_of("/");
  if (pos != string::npos) {
    return path.substr(pos + 1, path.length());
  } else {
    return path;
  }
}

} // namespace hort::filesystem
