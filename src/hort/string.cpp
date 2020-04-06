#include "hort/string.hpp"

namespace hort {

vector<string> string::split(const string& delims) const {
  vector<string> output{};
  std::size_t first = 0;
  while (first < this->size()) {
    const auto second = this->find_first_of(delims, first);
    if (first != second) {
      output.emplace_back(this->substr(first, second - first));
    }
    if (second == std::string::npos) break;
    first = second + 1;
  }
  return output;
}

string string::replace(const string& from, const string& to) const {
  string str    = *this;
  std::size_t n = 0;
  while ((n = str.find(from, n)) != std::string::npos) {
    str.replace(n, from.size(), to);
    n += to.size();
  }
  return str;
}

} // namespace hort
