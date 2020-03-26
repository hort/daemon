#include "hort/regexp.hpp"

namespace hort::regexp {

namespace detail {

Match findall(const re2::RE2& re, const std::string& target) {
  Match match;

  auto ngroups = re.NumberOfCapturingGroups();
  std::vector<re2::RE2::Arg> argv(ngroups);
  std::vector<re2::RE2::Arg*> args(ngroups);
  std::vector<re2::StringPiece> ws(ngroups);
  for (auto i = 0; i < ngroups; ++i) {
    args[i] = &argv[i];
    argv[i] = &ws[i];
  }

  re2::StringPiece input{target};
  while (re2::RE2::FindAndConsumeN(&input, re, &(args[0]), ngroups)) {
    for (const auto& w : ws) {
      match.emplace_back(w);
    }
  }

  return match;
}

Match search(const re2::RE2& re, const std::string& target) {
  Match match;

  auto ngroups = re.NumberOfCapturingGroups();
  std::vector<re2::RE2::Arg> argv(ngroups);
  std::vector<re2::RE2::Arg*> args(ngroups);
  std::vector<re2::StringPiece> ws(ngroups);
  for (auto i = 0; i < ngroups; ++i) {
    args[i] = &argv[i];
    argv[i] = &ws[i];
  }

  re2::StringPiece input{target};
  re2::RE2::PartialMatchN(input, re, &(args[0]), ngroups);
  for (const auto& w : ws) {
    match.emplace_back(w);
  }

  return match;
}

bool match(const re2::RE2& re, const std::string& target) {
  re2::StringPiece input{target};
  return re2::RE2::FullMatch(input, re);
}

} // namespace detail

Match find(const std::string& pattern, const std::string& target) {
  re2::RE2 re(pattern);
  return detail::find(re, target);
}

Match findall(const std::string& pattern, const std::string& target) {
  re2::RE2 re(pattern);
  return detail::findall(re, target);
}

bool match(const std::string& pattern, const std::string& target) {
  re2::RE2 re(pattern);
  return detail::match(re, target);
}

} // namespace hort::regexp
