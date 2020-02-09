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
[[nodiscard]] inline Match findall(const std::string &pattern, const std::string &target)
{
	re2::RE2 re(pattern);
	return detail::findall(re, target);
}

/// \brief Find first occurence of "pattern" in "target".
[[nodiscard]] inline Match search(const std::string &pattern, const std::string &target)
{
	re2::RE2 re(pattern);
	return detail::search(re, target);
}

/// \brief Check if "pattern" matches "target".
[[nodiscard]] inline bool match(const std::string &pattern, const std::string &target)
{
	re2::RE2 re(pattern);
	return detail::match(re, target);
}

/// \brief Find first occurence of "pattern" in "target" and save the results
/// inside the variadic arguments.
template <typename ...Args>
inline void search(const std::string &pattern, const std::string &target, Args& ...args)
{
	re2::RE2 re{pattern};
	detail::search(re, target, args...);
}

/// \brief Replace first occurence of "re" in "str" with "rewrite".
inline void replace(const std::string &pattern, const std::string &rewrite, std::string &str)
{
	re2::RE2::Replace(&str, pattern, rewrite);
}

/// \brief Replace all occurences of "re" in "str" with "rewrite".
inline void replace_all(const std::string &pattern, const std::string &rewrite, std::string &str)
{
	re2::RE2::GlobalReplace(&str, pattern, rewrite);
}

/// \brief Class to prevent having to recompile of the same regex more than once.
class Regex
{
	re2::RE2 r;

	static re2::RE2::Options &build_options()
	{
		static re2::RE2::Options o{};
		o.set_dot_nl(true);
		return o;
	}

	static const re2::RE2::Options &create_options()
	{
		static const auto o = build_options();
		return o;
	}

	const re2::RE2::Options &options = create_options();
	const std::string pattern;

public:
	Regex(const std::string &pattern_)
		: r{pattern_, create_options()}
		, pattern{pattern_} {}

	Regex(const Regex &other)
		: r{other.pattern, other.options}
		, pattern{other.pattern} {}

	/// \brief Find all occurence of "pattern" in "target".
	[[nodiscard]] Match findall(const std::string &target) const
	{
		return detail::findall(r, target);
	}

	/// \brief Find first occurence of "pattern" in "target".
	[[nodiscard]] Match search(const std::string &target) const
	{
		return detail::search(r, target);
	}

	/// \brief Check if "pattern" matches "target".
	[[nodiscard]] bool match(const std::string &target) const
	{
		return detail::match(r, target);
	}

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
