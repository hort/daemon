#ifndef RE_HPP_
#define RE_HPP_

#include <re2/re2.h>
#include <vector>

namespace hort::re
{

class Match final : public std::vector<std::string>
{
public:
	Match() : std::vector<std::string>{} {}

	explicit operator bool() const { return size(); }
	std::string last() const { return (*this)[this->size() - 1]; }
};

namespace detail
{

Match findall(const re2::RE2 &re, const std::string &target);
Match search(const re2::RE2 &re, const std::string &target);
bool  match(const re2::RE2 &re, const std::string &target);

template <typename ...Args>
inline void search(const re2::RE2 &re, const std::string &target, Args& ...args)
{
	re2::StringPiece input{target};
	re2::RE2::FullMatch(input, re, *args...);
}

} // namespace detail

/// \brief Find all occurence of "pattern" in "target".
[[nodiscard]] Match findall(const std::string &pattern, const std::string &target);

/// \brief Find first occurence of "pattern" in "target".
[[nodiscard]] Match search(const std::string &pattern, const std::string &target);

/// \brief Find first occurence of "pattern" in "target" and save the results
/// inside the variadic arguments.
template <typename ...Args>
inline void search(const std::string &pattern, const std::string &target, Args& ...args)
{
	re2::RE2 re{pattern};
	detail::search(re, target, args...);
}

/// \brief Check if "pattern" matches "target".
[[nodiscard]] bool match(const std::string &pattern, const std::string &target);

/// \brief Replace first occurence of "re" in "str" with "rewrite".
const auto replace = re2::RE2::Replace;

/// \brief Replace all occurences of "re" in "str" with "rewrite".
const auto replace_all = re2::RE2::GlobalReplace;

/// \brief Class to prevent having to recompile of the same regex more than once
class Regex
{
	re2::RE2 r;

	static const re2::RE2::Options &create_options()
	{
		static re2::RE2::Options os{};
		os.set_dot_nl(true);
		return os;
	}

	const re2::RE2::Options &options = create_options();
	const std::string pattern;

public:
	Regex(const std::string &pattern_);
	Regex(const Regex &other);
	//Regex(Regex &&other);

	/// \brief Find all occurence of "pattern" in "target".
	[[nodiscard]] Match findall(const std::string &target) const;

	/// \brief Find first occurence of "pattern" in "target".
	[[nodiscard]] Match search(const std::string &target) const;

	/// \brief Check if "pattern" matches "target".
	[[nodiscard]] bool match(const std::string &target) const;

	/// \brief Find first occurence of "pattern" in "target" and save the
	/// results inside the variadic arguments.
	template <typename ...Args>
	void search(const std::string &target, Args& ...args) const
	{
		detail::search(r, target, args...);
	}

};

} // namespace hort::re

#endif /* RE_HPP_ */
