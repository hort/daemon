#include "hort/http/cookiejar.hpp"

#include <fstream>

namespace hort::http
{

CookieJar::CookieJar(std::string filepath_)
	: filepath{filepath_}
	, cached{""} {}

void CookieJar::parse(struct curl_slist *unparsed)
{
	while (unparsed != nullptr) {
		std::string cookie = std::string(unparsed->data);
		int l = find_nth(cookie, '\t', 4, 0);
		int m = find_nth(cookie, '\t', 1, l);
		data[cookie.substr(l + 1, m - l - 1)] = cookie.substr(m + 1);
		unparsed = unparsed->next;
	}
	changed = true;
}

const std::string &CookieJar::serialize()
{
	char *escaped;
	if (changed) {
		cached = "";
		for (const auto &[k, v] : data) {
			if (v == "") continue;
			cached += k;
			cached += "=";
			cached += v;
			cached += "; ";
		}
		if (!cached.empty())
			cached.pop_back();
		changed = false;
	}
	return cached;
}

void CookieJar::save()
{
	std::ofstream file;
	file.open(filepath);
	for (const auto &cookie : data)
		file << cookie.first << '\t' << cookie.second << '\n';
	file << std::flush;
	file.close();
}

void CookieJar::load()
{
	std::ifstream file;
	file.open(filepath);
	std::string cookie;
	while (std::getline(file, cookie)) {
		int m = cookie.find_first_of('\t');
		data[cookie.substr(0, m)] =  cookie.substr(m + 1);
	}
	file.close();
}

std::string CookieJar::get(const std::string &key) const
{
	if (data.count(key))
		return data.at(key);
	return "";
}

void CookieJar::set(const std::string &key, std::string value)
{
	if (value == "") return;
	changed = true;
	data[key] = value;
}

void CookieJar::erase(const std::string &key)
{
	changed = true;
	data.erase(key);
}

void CookieJar::clear()
{
	changed = true;
	data.clear();
}

int CookieJar::find_nth(const std::string &str, char delim, int n, int s)
{
	int current  = s;
	int previous = 0;
	int index    = 0;
	if (!s) current = str.find(delim);
	while (current != std::string::npos && index < n) {
		previous = current + 1;
		current = str.find(delim, previous);
		++index;
	}
	return current;
}

} // namespace hort::http
