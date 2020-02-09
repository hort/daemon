#ifndef FZF_HPP_
#define FZF_HPP_

#include <string>
#include <vector>

namespace hort {

namespace detail
{

/// \brief Execute a command and return it's output.
std::string exec(const std::string &cmd);

//// \brief Split string into vector by delim
std::vector<std::string> split(const std::string &str, const std::string &delims);

[[nodiscard]] inline std::string fzf(const std::string &str)
{
	return detail::exec("echo -e '" + str + "' | fzf -m");
}

} // namespace detail

/// \brief Given a list strings, pass it to fzf and return the result.
[[nodiscard]] std::vector<std::string> fzf(const std::vector<std::string> &entries);

} // namespace hort

#endif /* FZF_HPP_ */
