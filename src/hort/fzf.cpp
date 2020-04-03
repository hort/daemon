#include "hort/fzf.hpp"

#include <array>
#include <iostream>
#include <memory>

namespace hort {

vector<string> fzf(const vector<string>& entries) {
  string str = "";
  for (const auto& entry : entries) {
    for (const auto& c : entry) {
      switch (c) {
      case '\\':
        str += "\\\\";
        break;
      case '\'':
        str += "'\\''";
        break;
      default:
        str += c;
        break;
      }
    }
    str += "\n";
  }
  str.pop_back();
  return detail::fzf(str).split("\n");
}

namespace detail {

string exec(const string& cmd) {
  std::array<char, 512> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.data(), "r"), pclose);
  if (!pipe) {
    return "";
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

} // namespace detail

} // namespace hort
