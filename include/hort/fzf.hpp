#ifndef FZF_HPP_
#define FZF_HPP_

#include <string>
#include <vector>

/// \brief Given a list strings, pass it to fzf and return the result.
std::vector<std::string> fzf(std::vector<std::string> elems);

/// \brief Given a list of newline separated strings, pass it to fzf and return
/// the result.
std::vector<std::string> fzf(std::string elems);

namespace detail
{

/// \brief Execute a command and return it's output.
std::string exec(std::string cmd);

//// \brief Split string into vector by delim
std::vector<std::string> split(const std::string &str, const std::string &delims);

} // namespace detail

#endif /* FZF_HPP_ */
