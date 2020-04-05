#include "hort/args.hpp"

namespace hort {

void Args::add(const std::string& s,
               const std::string& l,
               const std::string& d,
               const std::function<void()>& callback) {
  options.emplace_back(Argument{s, l, d, callback});
  if (max_width < l.size()) {
    max_width = l.size();
  }
}

void Args::add(const std::string& s,
               const std::string& l,
               const std::string& d,
               const std::function<void(int)>& callback) {
  options.emplace_back(Argument{s, l, d, callback});
  if (max_width < l.size()) {
    max_width = l.size();
  }
}

void Args::add(const std::string& s,
               const std::string& l,
               const std::string& d,
               const std::function<void(std::string)>& callback) {
  options.emplace_back(Argument{s, l, d, callback});
  if (max_width < l.size()) {
    max_width = l.size();
  }
}

void Args::parse(int argc, char* argv[]) {
  for (int i = 1; i < argc; i++) {
    bool flag = false;
    std::string arg = argv[i];
    for (const auto& o : options) {
      if ((flag |= (arg == o.s || arg == o.l))) {
        call_if_signature<void()>(o);
        call_if_signature<void(int)>(o, std::stoi(argv[++i]));
        call_if_signature<void(std::string)>(o, argv[++i]);
      }
    }
    if (!flag) {
      print("Unrecognized argument: {}"_format(arg));
      if (exit_on_fail) {
        exit(1);
      }
    }
  }
}

void Args::help() {
  fmt::print("Usage: {}", name);
  if (options.size())
    fmt::print(" [options...]");
  fmt::print("\n\n  {}\n", description);
  if (options.size()) {
    fmt::print("\nOptions:\n\n");
    for (const auto& o : options) {
      if (o.s.empty()) {
        fmt::print("      {:<" + std::to_string(max_width + 2) + "} {}\n", o.l, o.description);
      } else if (o.l.empty()) {
        fmt::print("  {}  {:<" + std::to_string(max_width + 2) + "} {}\n", o.s, "", o.description);
      } else {
        fmt::print("  {:<2}, {:<" + std::to_string(max_width + 2) + "} {}\n", o.s, o.l, o.description);
      }
    }
  }
  exit(1);
}

}
