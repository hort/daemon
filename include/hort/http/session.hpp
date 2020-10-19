#ifndef HORT_HTTP_SESSION_HPP_
#define HORT_HTTP_SESSION_HPP_

#include "hort/http/cookiejar.hpp"
#include "hort/http/response.hpp"

#include "hort/filesystem.hpp"
#include "hort/print.hpp"

#include <fmt/format.h>

namespace hort::http {

class Session {
  enum class Method { GET, POST };

  CURL* handle;
  int max_retries;
  int retry_delay;

  /// \brief CURL callback writer.
  static size_t writer(char* ptr, size_t size, size_t nmemb, std::string* data) {
    if (data == nullptr) {
      return 0;
    }
    data->append(ptr, size * nmemb);
    return size * nmemb;
  }

  /// \brief Parse incoming headers and save them inside `response`.
  void parse_headers(Response& response, const std::string& unparsed_headers);

  /// \brief Perform an HTTP request.
  Response request(const std::string& url,
                   const std::string& payload,
                   Method method);

public:
  explicit Session(const std::string& cookie_filepath = "./cookies");

  ~Session() {
    curl_easy_cleanup(handle);
    cookies.save();
  }

  /// \brief Send a GET request to `url`.
  Response get(const std::string& url) {
    return request(url, "", Method::GET);
  }

  /// \brief Send a GET request to `url` with formatted `params`.
  Response get(const std::string url,
               const std::map<std::string, std::string>& params);

  /// \brief Send a GET request to `url` with formatted `params`.
  template <typename... Args>
  Response get(const std::string& format,
               const Args&... args,
               const std::map<std::string, std::string>& params) {
    fmt::format_args argspack = fmt::make_format_args(args...);
    auto url = fmt::vformat(format, argspack);
    return get(url, "", params);
  }

  /// \brief Send a GET request to `url`.
  template <typename... Args>
  Response get(const std::string& format, const Args&... args) {
    fmt::format_args argspack = fmt::make_format_args(args...);
    auto url = fmt::vformat(format, argspack);
    return request(url, "", Method::GET);
  }

  /// \brief Send a POST request to `url` with `payload`.
  Response post(const std::string& url, const std::string& payload) {
    return request(url, payload, Method::POST);
  }

  /// \brief Send a POST request to `url` with `payload`.
  Response post(const std::string url,
                const std::map<std::string, std::string>& payload);

  /// \brief Download resource at `url` and save response to file.
  /// \brief Returns written bytes to memory, -1 if none.
  int download(const std::string& filepath,
               const std::string& filename,
               const std::string& url) {
    auto response = get(url);
    if (response.code != 200) return -1;
    filesystem::write(response.body, filepath, filename);
    return !response.body.size();
  }

  /// \brief Download resource at `url` and save response to file.
  template <typename... Args>
  int download(const std::string& filepath,
               const std::string& filename,
               const std::string& format,
               const Args&... args) {
    fmt::format_args argspack = fmt::make_format_args(args...);
    auto url = fmt::vformat(format, argspack);
    return download(filepath, filename, url);
  }

  /// \brief Default headers for all request.
  std::map<std::string, std::string> headers;

  /// \brief Cookies for the current session.
  CookieJar cookies;
};

inline Response get(const std::string& url) {
  Session session;
  return session.get(url);
}

template <typename ... Args>
int download(const std::string& filepath,
             const std::string& filename,
             const std::string& format,
             const Args&... args) {
  Session session;
  fmt::format_args argspack = fmt::make_format_args(args...);
  auto url = fmt::vformat(format, argspack);
  return session.download(filepath, filename, url);
}

} // namespace hort::http

#endif /* HORT_HTTP_SESSION_HPP_ */
