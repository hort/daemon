#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>

#include "hort/http/session.hpp"
#include "hort/filesystem.hpp"

namespace hort::http
{

Session::Session(const std::string &cookie_filepath)
	: cookies{cookie_filepath}
	, headers{}
	, max_retries{5}
	, retry_delay{5}
{
	handle = curl_easy_init();

	curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "");
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, true);
	curl_easy_setopt(handle, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
	curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 1L);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 1L);
	curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 0L);

	headers["Accept"]          = "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3";
	headers["Accept-Language"] = "en-US,en;q=0.9";
	headers["User-Agent"]      = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3865.90 Safari/537.36";
}

Session::~Session()
{
	curl_easy_cleanup(handle);
}

size_t Session::writer(char *ptr, size_t size, size_t nmemb, std::string *data)
{
	if (data == NULL)
		return 0;
	data->append(ptr, size * nmemb);
	return size * nmemb;
}

void Session::parse_headers(Response &response, const std::string &unparsed)
{
	const char delim = '\n';

	int pos      = 0;
	int current  = 0;
	int previous = 0;

	std::string header;
	std::string key;
	std::string value;

	current = unparsed.find(delim);

	while (current != std::string::npos) {
		// get the current header
		header = unparsed.substr(previous, current - previous);

		// split it into key and value
		pos = header.find(":");
		if (pos == std::string::npos) return;
		key   = header.substr(0, pos);
		value = header.substr(pos + 2);
		response.headers[key] = value;

		// move to the next header
		previous = current + 1;
		current = unparsed.find(delim, previous);
	}
}

Response Session::get(const std::string &url)
{
	return request(url, "", Method::GET);
}

Response Session::get(const std::string url, const std::map<std::string, std::string> &params)
{
	std::string parsed_params;
	char *escaped_field;
	for (const auto &field : params) {
		escaped_field = curl_easy_escape(handle, field.second.c_str(), field.second.size());
		parsed_params += field.first;
		parsed_params += "=";
		parsed_params += std::string(escaped_field);
		parsed_params += "&";
		if (escaped_field)
			curl_free(escaped_field);
	}
	parsed_params.pop_back();
	return request(url + parsed_params, "", Method::GET);
}

Response Session::post(const std::string &url, const std::string &payload)
{
	return request(url, payload, Method::POST);
}

Response Session::post(const std::string url, const std::map<std::string, std::string> &payload)
{
	std::string parsed_payload;
	char *escaped_field;
	for (const auto &field : payload) {
		escaped_field = curl_easy_escape(handle, field.second.c_str(), field.second.size());
		parsed_payload += field.first;
		parsed_payload += "=";
		parsed_payload += std::string(escaped_field);
		parsed_payload += "&";
		if (escaped_field)
			curl_free(escaped_field);
	}
	parsed_payload.pop_back();
	return request(url, parsed_payload, Method::POST);
}

Response Session::request(const std::string &url, const std::string &payload, Method method)
{
	Response response;
	std::string response_body;
	std::string response_headers;

	CURLcode res;
	std::string parsed_cookies   = "";
	struct curl_slist *unparsed_cookies = NULL;
	char *effective_url    = NULL;

	curl_easy_setopt(handle, CURLOPT_URL, url.data());

	// define sinks for response headers and body
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(handle, CURLOPT_HEADERDATA, &response_headers);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA,  &response_body);

	// compile header hash map to a curl_slist
	struct curl_slist *parsed_headers = NULL;
	for (const auto &[k, v] : headers) {
		parsed_headers = curl_slist_append(parsed_headers, (k + ": " + v).c_str());
	}

	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, parsed_headers);

	if(method == Method::POST) {
		curl_easy_setopt(handle, CURLOPT_POST, 1);
		curl_easy_setopt(handle, CURLOPT_POSTFIELDS, payload.data());
	}

	// compile cookies
	auto compiled = cookies.serialize();
	curl_easy_setopt(handle, CURLOPT_COOKIE, compiled.c_str());

	// retry up to max_retrier, wait retry_delay each time
	res = curl_easy_perform(handle);
	if(res == CURLE_OK) goto success;
	for (int i = 0; i < max_retries - 1; i++) {
		usleep(retry_delay * 1000 * 1000);
		res = curl_easy_perform(handle);
		if (res == CURLE_OK) break;
	}

	return response;

	success:

	// handle the response
	curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response.code);
	curl_easy_getinfo(handle, CURLINFO_COOKIELIST, &unparsed_cookies);
	curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &effective_url);

	// parse cookies and headers
	cookies.parse(unparsed_cookies);
	parse_headers(response, response_headers);

	response.body = response_body;
	response.url = std::string(effective_url);

	return response;
}

int Session::download(const std::string &filepath, const std::string &filename, const std::string &url)
{
	hort::filesystem::mkpath(filepath);
	auto bin = get(url).body;
	return hort::filesystem::save(bin, filepath, filename);
}

} // namespace hort::http
