#include "hort/http/cookiejar.hpp"

#include <fstream>

namespace hort::http {

std::string CookieJar::get(const std::string& key) const {
  if (data.count(key)) {
    return data.at(key);
  }
  return "";
}

std::string &CookieJar::operator[](const std::string key) {
  return data[key];
}

void CookieJar::set(const std::string& key, const std::string &value) {
  if (value.empty()) return;
  changed   = true;
  data[key] = value;
}

void CookieJar::erase(const std::string& key) {
  changed = true;
  data.erase(key);
}

void CookieJar::clear() {
  changed = true;
  data.clear();
}

void CookieJar::parse(struct curl_slist* unparsed) {
  for (; unparsed != nullptr; unparsed = unparsed->next) {
    auto cookie = std::string(unparsed->data);
    auto l = find_nth(cookie, '\t', 4, 0);
    auto m = find_nth(cookie, '\t', 1, l);
    data[cookie.substr(l + 1, m - l - 1)] = cookie.substr(m + 1);
  }
  changed = true;
}

const std::string& CookieJar::serialize() {
  if (changed) {
    cached = "";
    for (const auto& [k, v] : data) {
      if (v.empty()) continue;
      cached += k;
      cached += "=";
      cached += v;
      cached += "; ";
    }
    if (!cached.empty()) {
      cached.pop_back();
      cached.pop_back();
    }
    changed = false;
  }
  return cached;
}

void CookieJar::save() {
  std::ofstream file;
  file.open(filepath);
  for (const auto& [k, v] : data) {
    file << k << '\t' << v << '\n';
  }
  file << std::flush;
  file.close();
}

void CookieJar::load() {
  std::ifstream file;
  file.open(filepath);
  std::string cookie;
  while (std::getline(file, cookie)) {
    auto m = cookie.find_first_of('\t');
    data[cookie.substr(0, m)] = cookie.substr(m + 1);
  }
  file.close();
}

int CookieJar::find_nth(const std::string& str, char delim, int n, int s) {
  std::string::size_type current  = s;
  int index                       = 0;
  if (!s)
    current = str.find(delim);
  while (current != std::string::npos && index < n) {
    auto previous = current + 1;
    current  = str.find(delim, previous);
    ++index;
  }
  return current;
}

} // namespace hort::http
