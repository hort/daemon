#ifndef HORT_ARGS_HPP_
#define HORT_ARGS_HPP_

#include <functional>
#include <map>
#include <string>
#include <variant>

#include "hort/print.hpp"

namespace hort {

// TODO support numeric and string list arguments
// TODO add support for variable binding
class Args {

  struct Argument {
    const std::string s;
    const std::string l;
    const std::string description;

    const std::variant<
      std::reference_wrapper<bool>,
      std::reference_wrapper<int>,
      std::reference_wrapper<std::string>,
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
  std::size_t max_width = 0;

  bool exit_on_fail = false;

  template <typename T, typename ... Args>
  static bool call_if_signature(const Argument &o, const Args & ... args) {
    if (std::holds_alternative<std::function<T>>(o.callback)) {
      std::get<std::function<T>>(o.callback)(args...);
      return true;
    }
    return false;
  }

  template <typename T, typename U>
  static bool assign_if_reference(const Argument& o, const U& u) {
    if (std::holds_alternative<std::reference_wrapper<T>>(o.callback)) {
      std::get<std::reference_wrapper<T>>(o.callback).get() = u;
      return true;
    }
    return false;
  }

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
           const std::reference_wrapper<bool>& value);

  void add(const std::string& s,
           const std::string& l,
           const std::string& d,
           const std::reference_wrapper<int>& value);

  void add(const std::string& s,
           const std::string& l,
           const std::string& d,
           const std::reference_wrapper<std::string>& value);

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

  void help();

};

} // namespace hort

#endif /* HORT_ARGS_HPP_ */
