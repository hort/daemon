#include "hort/fzf.hpp"

#include <array>
#include <memory>

namespace hort
{

namespace detail
{

std::string exec(std::string cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.data(), "r"), pclose);
    if (!pipe)
        throw std::runtime_error("popen() failed!");
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

std::vector<std::string> fzf(std::string elems)
{
	std::string s = detail::exec("echo -e \"" + elems + "\" | fzf -m");
	return detail::split(s, "\n");
}

std::vector<std::string> fzf(std::vector<std::string> elems)
{
	std::string es = "";
	for (const auto &e : elems) {
		es += e;
		es += "\n";
	}
	es.pop_back();
	return fzf(es);
}

} // namespace hort
