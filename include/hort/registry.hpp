#ifndef HORT_REGISTRY_HPP_
#define HORT_REGISTRY_HPP_

#include <type_traits>
#include <typeinfo>

#include "hort/args.hpp"
#include "hort/interface.hpp"
#include "hort/input.hpp"

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
      for (auto & i : name) {
        if (i >= 'A' && i <= 'Z') i += 32;
      }
    }

    return name;
  }

  std::map<string, std::unique_ptr<Interface>> interfaces{};
  std::map<string, vector<string>> methods{};
  Args argparser{"hortd", "Data hoarding and indexing framework", "0.1.0"};

public:
  Registry() {
    argparser.add("-r", "--repl", "Run REPL", [this]() {
      repl();
    });
    argparser.add("-f", "--forward", "Forward", [this](const std::string &s) {
      forward(s);
    });
    curl_global_init(CURL_GLOBAL_DEFAULT);
  }

  ~Registry() { curl_global_cleanup(); }

  static Registry* instance() {
    static std::unique_ptr<Registry> registry;
    if (registry == nullptr) {
      registry = std::make_unique<Registry>();
    }
    return registry.get();
  }

  void parse(int argc, char* argv[]);

  template <typename T>
  constexpr void add() {
    auto name = get_type_name_lower<T>();
    interfaces.emplace(make_pair(name, std::make_unique<T>()));
  }

  template <typename T>
  constexpr void method(const std::string &description) {
    auto name = get_type_name_lower<T>();
    methods[name].push_back(description);
  }

  void repl();

  void auth();

  void forward(const std::string &s);

};

template <typename T>
struct AutoRegistry {
  constexpr AutoRegistry() {
    Registry::instance()->add<T>();
  }

  constexpr void *method() {
    Registry::instance()->method<T>("Hello, World!");
    return nullptr;
  }
};

} // namespace hort

#endif /* HORT_REGISTRY_HPP_ */
