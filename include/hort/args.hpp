#ifndef HORT_ARGS_HPP_
#define HORT_ARGS_HPP_

#include <fmt/format.h>
#include <map>
#include <string>
#include <variant>

#include "hort/print.hpp"

namespace hort {

// TODO support numeric and string list arguments
class Args {

  struct Argument {
    const std::string s;
    const std::string l;
    const std::string description;

    const std::variant<
        std::function<void()>,
        std::function<void(int)>,
        std::function<void(std::string)>
      > callback;
  };

  std::vector<Argument> options{};

  const std::string name;
  const std::string description;
  const std::string version;

  /// \brief Width of the longest option flag.
  /// \see hort::Args::help for use.
  int  max_width    = 0;
  bool exit_on_fail = false;

  template <typename T, typename ... Args>
  static void call_if_signature(const Argument &o, const Args & ... args) {
    if (std::holds_alternative<std::function<T>>(o.callback))
      std::get<std::function<T>>(o.callback)(args...);
  }

  void help();

public:
  Args(const std::string &name_, const std::string &description_, const std::string &version_)
      : name{name_}, description{description_}, version{version_} {
    add("-h", "--help", "Show help", [this]() { help(); });
    add("-v", "--version", "Show version", [this]() {
      print("{} version {}"_format(name, version));
      exit(1);
    });
  }

  void add(const std::string& s,
           const std::string& l,
           const std::string& d,
           const std::function<void()>& callback);

  void add(const std::string& s,
           const std::string& l,
           const std::string& d,
           const std::function<void(int)>& callback);

  void add(const std::string& s,
           const std::string& l,
           const std::string& d,
           const std::function<void(std::string)>& callback);

  void parse(int argc, char* argv[]);

};

} // namespace hort

#endif /* HORT_ARGS_HPP_ */
