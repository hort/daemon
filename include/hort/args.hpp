#ifndef __HORT_ARGS_HPP_
#define __HORT_ARGS_HPP_

#include <functional>
#include <map>
#include <string>
#include <variant>

#include "hort/print.hpp"

namespace hort {

// Automatically generates help and version flags.
//
// TODO support for numeric and string lists
class Args {
private:
   
  struct Argument {
    const std::string_view s;
    const std::string_view l;
    const std::string_view description;

    const std::variant<
      std::reference_wrapper<bool>,
      std::reference_wrapper<int>,
      std::reference_wrapper<std::string>,
      std::function<void()>,
      std::function<void(int)>,
      std::function<void(std::string)>> value;
  };

  std::vector<Argument> options{};

  const std::string_view name;
  const std::string_view description;
  const std::string_view version;

  bool exit_on_fail = true;

  /// \brief Width of the longest option flag, used in printing help menu.
  std::size_t max_width = 0;

public:
  Args(const std::string_view &name_,
       const std::string_view &description_,
       const std::string_view &version_)
      : name{name_}
      , description{description_}
      , version{version_} {
    add("-h", "--help", "Show help", [this]() { help(); });
    add("-v", "--version", "Show version", [this]() {
      hort::printf("{} version {}\n", name, version);
      exit(1);
    });
  }

  void add(const std::string_view& s,
           const std::string_view& l,
           const std::string_view& d,
           const std::reference_wrapper<bool>& value);

  void add(const std::string_view& s,
           const std::string_view& l,
           const std::string_view& d,
           const std::reference_wrapper<int>& value);

  void add(const std::string_view& s,
           const std::string_view& l,
           const std::string_view& d,
           const std::reference_wrapper<std::string>& value);

  void add(const std::string_view& s,
           const std::string_view& l,
           const std::string_view& d,
           const std::function<void()>& callback);

  void add(const std::string_view& s,
           const std::string_view& l,
           const std::string_view& d,
           const std::function<void(int)>& callback);

  void add(const std::string_view& s,
           const std::string_view& l,
           const std::string_view& d,
           const std::function<void(std::string)>& callback);

  void help();

  void parse(int argc, char* argv[]);
};

} // namespace hort

#endif /* __HORT_ARGS_HPP_ */
