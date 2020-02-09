#include "hort/fzf.hpp"
#include "hort/re.hpp"

#include <array>
#include <memory>
#include <iostream>

namespace hort
{

std::vector<std::string> fzf(const std::vector<std::string> &entries)
{
	std::string str = "";
	for (const auto &entry : entries) {
		for (const auto &c : entry) {
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
	return detail::split(detail::fzf(str), "\n");
}

namespace detail
{

std::string exec(const std::string &cmd)
{
	std::array<char, 512> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.data(), "r"), pclose);
	if (!pipe) return "";
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
		result += buffer.data();
	return result;
}

std::vector<std::string> split(const std::string &str, const std::string &delims)
{
	std::vector<std::string> output;
	int first = 0;
	while (first < str.size()) {
		const auto second = str.find_first_of(delims, first);
		if (first != second)
			output.emplace_back(str.substr(first, second - first));
		if (second == std::string::npos)
			break;
		first = second + 1;
	}
	return output;
}

} // namespace detail

} // namespace hort
