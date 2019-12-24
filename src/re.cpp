#include "re.hpp"

namespace hort::re
{

Match findall(const std::string &pattern, const std::string &target)
{
	re2::RE2 re(pattern);
	return detail::findall(re, target);
}

Match search(const std::string &pattern, const std::string &target)
{
	re2::RE2 re(pattern);
	return detail::search(re, target);
}

bool match(const std::string &pattern, const std::string &target)
{
	re2::RE2 re(pattern);
	return detail::match(re, target);
}

namespace detail
{

Match findall(const re2::RE2 &re, const std::string &target)
{
	// thanks to this gist: https://gist.github.com/chezou/1395527
	Match match;
	re2::StringPiece input(target);

	unsigned int ngroups = re.NumberOfCapturingGroups();
	std::vector<re2::RE2::Arg> argv(ngroups);
	std::vector<re2::RE2::Arg*> args(ngroups);
	std::vector<re2::StringPiece> ws(ngroups);
	for (unsigned int i = 0; i < ngroups; ++i) {
		args[i] = &argv[i];
		argv[i] = &ws[i];
	}

	while (re2::RE2::FindAndConsumeN(&input, re, &(args[0]), ngroups))
		for (const auto &w : ws)
			match.emplace_back(w);

	return match;
}

Match search(const re2::RE2 &re, const std::string &target)
{
	Match match;
	re2::StringPiece input(target);

	unsigned int ngroups = re.NumberOfCapturingGroups();
	std::vector<re2::RE2::Arg> argv(ngroups);
	std::vector<re2::RE2::Arg*> args(ngroups);
	std::vector<re2::StringPiece> ws(ngroups);
	for (unsigned int i = 0; i < ngroups; ++i) {
		args[i] = &argv[i];
		argv[i] = &ws[i];
	}

	re2::RE2::PartialMatchN(input, re, &(args[0]), ngroups);
	for (const auto &w : ws)
		match.emplace_back(w);

	return match;
}

bool match(const re2::RE2 &re, const std::string &target)
{
	re2::StringPiece input(target);
	return re2::RE2::FullMatch(input, re);
}

} // namespace detail

Regex::Regex(const std::string &pattern)
	: r{pattern, options} {}

[[nodiscard]] Match Regex::findall(const std::string &target)
{
	return detail::findall(r, target);
}

[[nodiscard]] Match Regex::search(const std::string &target)
{
	return detail::search(r, target);
}

[[nodiscard]] bool Regex::match(const std::string &target)
{
	return detail::match(r, target);
}

} // namespace hort::re
