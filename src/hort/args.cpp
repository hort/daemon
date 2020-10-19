#include "hort/args.hpp"

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace hort {

void Args::add(const std::string_view& s,
               const std::string_view& l,
               const std::string_view& d,
               const std::reference_wrapper<bool>& value) {
  options.emplace_back(Argument{s, l, d, value});
  if (max_width < l.size()) {
    max_width = l.size();
  }
}

void Args::add(const std::string_view& s,
               const std::string_view& l,
               const std::string_view& d,
               const std::reference_wrapper<int>& value) {
  options.emplace_back(Argument{s, l, d, value});
  if (max_width < l.size()) {
    max_width = l.size();
  }
}

void Args::add(const std::string_view& s,
               const std::string_view& l,
               const std::string_view& d,
               const std::reference_wrapper<std::string>& value) {
  options.emplace_back(Argument{s, l, d, value});
  if (max_width < l.size()) {
    max_width = l.size();
  }
}

void Args::add(const std::string_view& s,
               const std::string_view& l,
               const std::string_view& d,
               const std::function<void()>& callback) {
  options.emplace_back(Argument{s, l, d, callback});
  if (max_width < l.size()) {
    max_width = l.size();
  }
}

void Args::add(const std::string_view& s,
               const std::string_view& l,
               const std::string_view& d,
               const std::function<void(int)>& callback) {
  options.emplace_back(Argument{s, l, d, callback});
  if (max_width < l.size()) {
    max_width = l.size();
  }
}

void Args::add(const std::string_view& s,
               const std::string_view& l,
               const std::string_view& d,
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
    for (auto o : options) {
      if ((flag |= (arg == o.s || arg == o.l))) {
        std::visit(overloaded {
          [&](const std::function<void()> &f) {
            f();
            i += 1;
          },
          [&](const std::function<void(int)> &f) {
            f(std::atoi(argv[i + 1]));
            i += 2;
          },
          [&](const std::function<void(std::string)> &f) {
            f(argv[i + 1]);
            i += 2;
          },
          [&](const std::reference_wrapper<bool> &v) {
            v.get() = true;
            i += 1;
          },
          [&](const std::reference_wrapper<int> &v) {
            v.get() = std::atoi(argv[i + 1]);
            i += 2;
          },
          [&](const std::reference_wrapper<std::string> &v) {
            v.get() = argv[i + 1];
            i += 2;
          },
        }, o.value);
        if (i > argc) { break; }
      }
    }
    if (!flag) {
      hort::printf("Unrecognized argument: {}\n", arg);
      help();
      if (exit_on_fail) {
        exit(1);
      }
    }
  }
}

void Args::help() {
  hort::printf("Usage: {}", name);
  if (options.size()) {
    hort::print(" [options...]");
  }
  hort::printf("\n\n  {}\n", description);
  if (options.size()) {
    hort::print("\nOptions:\n\n");
    for (const auto& o : options) {
      if (o.s.empty()) {
        hort::printf("      {:<" + std::to_string(max_width + 2) + "} {}\n", o.l, o.description);
      } else if (o.l.empty()) {
        hort::printf("  {}  {:<" + std::to_string(max_width + 2) + "} {}\n", o.s, "", o.description);
      } else {
        hort::printf("  {:<2}, {:<" + std::to_string(max_width + 2) + "} {}\n", o.s, o.l, o.description);
      }
    }
  }
  exit(1);
}

} // namespace hort
