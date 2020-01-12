#ifndef HTTP_COOKIEJAR_HPP_
#define HTTP_COOKIEJAR_HPP_

#include <curl/curl.h>
#include <map>

namespace hort::http
{

/// \brief Handle HTTP session cookies.
class CookieJar
{

	std::map<std::string, std::string> data;
	std::string filepath;
	std::string cached;
	bool changed = true;

	/// \brief Find nth delim after in str after position s.
	int find_nth(const std::string &str, char delim, int n, int s = 0);

public:
	CookieJar(std::string filepath_);

	std::string get(const std::string &key) const;
	void set(const std::string &key, std::string value);
	void erase(const std::string &key);
	void clear();

	/// \brief Parse incoming cookies from a response.
	void parse(struct curl_slist *unparsed);

	/// \brief Serialize cookies to string. Caches if jar isn't modified.
	/// \return const reference to cached serialized jar.
	const std::string &serialize();

	/// \brief Save cookies to file.
	void load();

	/// \brief Load cookies from file.
	void save();

};

} // namespace hort::http

#endif /* HTTP_COOKIEJAR_HPP_ */
