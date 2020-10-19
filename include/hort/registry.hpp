#ifndef HORT_REGISTRY_HPP_
#define HORT_REGISTRY_HPP_

#include <type_traits>
#include <typeinfo>
#include <iostream>

#include "hort/args.hpp"
#include "hort/input.hpp"
#include "hort/interface.hpp"

#include "hort/repl/lexer.hpp"

#define CTRLKEY(k) ((k) & 0x1f)

template <class ... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class ... Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace hort {

class Registry {

  /// \brief Return the types name (all lowercase).
  template <typename T>
  static std::string& get_type_name_lower() {
    static std::string name;

    if (name.empty()) {
      name  = __PRETTY_FUNCTION__;
      int s = name.find("with T = ");
      int e = name.find(";", s);
      name  = name.substr(s + 9, e - s - 9);
      for (auto& i : name) {
        if (i >= 'A' && i <= 'Z')
          i += 32;
      }
    }

    return name;
  }

  std::map<string, std::unique_ptr<Interface>> interfaces{};

  Args argparser{"hortd", "Data hoarding and indexing framework", "0.1.0"};

  void initwin() {
    system("clear");
    system("stty raw");
    system("stty -echo");
    printf("\033[?25l");
  }

  void endwin() {
    system("clear");
    system("stty cooked");
    system("stty echo");
    printf("\033[?25h");
  }

public:
  Registry() {
    argparser.add("-i", "--interactive", "Run in interactive mode", [this]() { repl(); });
    argparser.add("-r", "--repl", "Run REPL (experimental)", [this]() {
      bool exit_request = false;
      std::string str = "";
      initwin();
      while (!exit_request) {
        switch (char c = getchar()) {
          case CTRLKEY('c'):
            exit_request = true;
            break;
          case 127:
            str.pop_back();
            break;
          default:
            str += c;
            hort::repl::Lexer l{str};
            hort::printf("> \r{}", l.colorized());
            std::fflush(stdout);
            break;
        }
      }
      endwin();
    });
    argparser.add("-f", "--forward", "Forward", [this](const std::string& s) { forward(s); });
    argparser.add("-d", "--dump", "Dump an interface's index", [this](const std::string& s) { print(interfaces[s]->dump()); });
    argparser.add("-a", "--archive", "Archive Interface", [this](const std::string& s) { interfaces[s]->archive(); });
    curl_global_init(CURL_GLOBAL_DEFAULT);
  }

  Registry(Registry&&) = delete;
  Registry(const Registry&) = delete;
  Registry& operator=(const Registry&) = delete;
  Registry&& operator=(Registry&&) = delete;

  ~Registry() { curl_global_cleanup(); }

  static Registry& instance() {
    static std::shared_ptr<Registry> registry = std::make_shared<Registry>();
    return *registry.get();
  }

  void parse(int argc, char* argv[]);

  void repl();

  void auth();

  void forward(const std::string& s);

  template <typename T>
  constexpr void add() {
    auto interface = std::make_unique<T>();
    interfaces.emplace(make_pair(interface->id, std::move(interface)));
  }
};

template <typename T>
struct AutoRegistry {
  AutoRegistry() { Registry::instance().add<T>(); }
};

} // namespace hort

#endif // HORT_REGISTRY_HPP_
