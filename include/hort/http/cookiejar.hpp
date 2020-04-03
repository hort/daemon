#ifndef HORT_HTTP_COOKIEJAR_HPP_
#define HORT_HTTP_COOKIEJAR_HPP_

#include <curl/curl.h>
#include <map>

namespace hort::http {

/// \brief Handle HTTP session cookies.
/// TODO: domain and expiration handling
class CookieJar {

  std::map<std::string, std::string> data;
  std::string filepath;
  std::string cached = "";
  bool changed = true;

  /// \brief Find nth delim after in str after position s.
  /// \param n nth position to check
  /// \param s starting position
  int find_nth(const std::string& str, char delim, int n, int s = 0);

public:
  explicit CookieJar(std::string filepath_) : filepath{filepath_} {};

  [[nodiscard]] std::string get(const std::string& key) const;

  [[nodiscard]] std::string &operator[](const std::string key);

  void set(const std::string& key, const std::string &value);

  void erase(const std::string& key);

  void clear();

  /// \brief Parse incoming cookies from a response.
  void parse(struct curl_slist* unparsed);

  /// \brief Serialize cookies to string. Caches if jar isn't modified.
  /// \return Cached serialized jar.
  const std::string& serialize();

  /// \brief Save cookies to file.
  void load();

  /// \brief Load cookies from file.
  void save();

};

} // namespace hort::http

#endif /* HORT_HTTP_COOKIEJAR_HPP_ */
