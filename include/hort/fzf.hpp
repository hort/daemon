#ifndef HORT_FZF_HPP_
#define HORT_FZF_HPP_

#include "hort/string.hpp"
#include "hort/vector.hpp"

namespace hort {

namespace detail {

/// \brief Execute a command and return it's output.
string exec(const string& cmd);

[[nodiscard]] inline string fzf(const std::string& str) {
  return detail::exec("echo -e '" + str + "' | fzf -m");
}

} // namespace detail

/// \brief Given a list strings, pass it to fzf and return the result.
[[nodiscard]] vector<string> fzf(const vector<string>& entries);

} // namespace hort

#endif /* HORT_FZF_HPP_ */
