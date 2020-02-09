#include "hort/re.hpp"

namespace hort::re::detail
{

Match findall(const re2::RE2 &re, const std::string &target)
{
	// thanks to: https://gist.github.com/chezou/1395527
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
	std::vector<re2::RE2::Arg>    argv(ngroups);
	std::vector<re2::RE2::Arg*>   args(ngroups);
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

} // namespace hort::re::detail
