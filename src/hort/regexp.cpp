#include "hort/regexp.hpp"

namespace hort::regexp::detail {

vector<string> findall(const re2::RE2& re, const std::string& target) {
  vector<string> match;

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

vector<string> search(const re2::RE2& re, const std::string& target) {
  vector<string> match;

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

} // namespace hort::regexp::detail
