#include "hort/filesystem.hpp"

#include <sys/stat.h>
#include <cstdio>
#include <fstream>
#include <unistd.h>

namespace hort::filesystem {

bool exists(const string& path) {
  struct stat buffer;
  return stat(path.data(), &buffer) == 0;
}

bool mkpath(string path) {
  if (exists(path)) {
    return 0;
  }

  if (path[path.size() - 1] != '/') {
    path += '/';
  }

  string::size_type pos = 0;
  while ((pos = path.find_first_of('/', pos)) != string::npos) {
    string dir = path.substr(0, pos++);
    if (dir.size() == 0) {
      continue;
    }
    if (auto mdret = mkdir(dir.c_str(), 0775); mdret && errno != EEXIST) {
      return mdret;
    }
  }

  return 0;
}

bool write(const string& source,
           const string& filepath,
           const string& filename) {
  mkpath(filepath);
  std::ofstream f{};
  f.open(filepath + "/" + filename);
  if (f.fail()) {
    return 1;
  }
  f << source;
  f.close();
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
