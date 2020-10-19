#ifndef HORT_HTTP_RESPONSE_HPP_
#define HORT_HTTP_RESPONSE_HPP_

#include "hort/formats.hpp"
#include "hort/re.hpp"

namespace hort::http {

struct Response {

  std::string body;
  std::string url;
  std::map<std::string, std::string> headers;
  long code;

  /// \brief Parse response body as JSON
  [[nodiscard]] json parse() {
    try {
      return json::parse(body);
    } catch (const json::basic_json::parse_error& e) { return nullptr; }
  };

  /// \brief Parse response body as XML
  [[nodiscard]] json feedparse() { return formats::xml2json(body); }

  [[nodiscard]] vector<string> findall(const std::string& pattern) {
    return re::findall(pattern, body);
  }
};

} // namespace hort::http

#endif /* HORT_HTTP_RESPONSE_HPP_ */
