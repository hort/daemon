#ifndef SESSION_HPP_
#define SESSION_HPP_

#include "hort/http/cookiejar.hpp"
#include "hort/http/response.hpp"

#include <fmt/format.h>

namespace hort::http
{

class Session
{
	friend class Form;

	enum class Method { GET, POST };

	CURL *handle;
	int max_retries;
	int retry_delay;

	/// \brief CURL callback writer.
	static size_t writer(char *ptr, size_t size, size_t nmemb, std::string *data);

	/// \brief Parse incoming headers and save them inside response.
	void parse_headers(Response &response, const std::string &unparsed_headers);

	/// \brief Perform an HTTP request.
	Response request(const std::string &uri, const std::string &payload, Method method);

public:
	Session(const std::string &cookie_filepath="./cookies");
	~Session();

	/// \brief Send a GET request to uri.
	Response get(const std::string &uri);

	/// \brief Send a GET request to uri with formatted params.
	Response get(const std::string uri, const std::map<std::string, std::string> &params);

	/// \brief Send a GET request to uri with formatted params.
	template <typename... Args>
	Response get(std::string_view format, const Args&... args, const std::map<std::string, std::string> &params)
	{
		auto uri = fmt::vformat(format, fmt::make_format_args(args...));
		return get(uri, "", params);
	}

	/// \brief Send a GET request to uri.
	template <typename... Args>
	Response get(fmt::string_view format, const Args&... args)
	{
		std::string uri = fmt::vformat(format, fmt::make_format_args(args...));
		return request(uri, "", Method::GET);
	}

	/// \brief Send a POST request to uri with payload.
	Response post(const std::string &uri, const std::string &payload);

	/// \brief Send a POST request to uri with payload.
	Response post(const std::string url, const std::map<std::string, std::string> &payload);

	/// \brief Download uri and save response to fill.
	int download(const std::string &filepath, const std::string &filename, const std::string &uri);

	/// \brief Download uri and save response to fill.
	template <typename... Args>
	int download(const std::string &filepath, const std::string &filename, std::string_view format, const Args&... args)
	{
		auto uri = fmt::vformat(format, fmt::make_format_args(args...));
		return download(filepath, filename, uri);
	}

	/// \brief Default headers for all request.
	std::map<std::string, std::string> headers;

	/// \brief Cookies for the current session.
	CookieJar cookies;
};

} // namespace hort::http

#endif /* SESSION_HPP_ */
