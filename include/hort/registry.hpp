#ifndef HORT_REGISTRY_HPP_
#define HORT_REGISTRY_HPP_

#include <type_traits>
#include <typeinfo>

#include "hort/args.hpp"
#include "hort/interface.hpp"
#include "hort/input.hpp"

namespace hort {

class Registry {

public:
  Registry()
      : interfaces{}
      , argparser{"hortd", "Data hoarding and indexing framework"} {
    curl_global_init(CURL_GLOBAL_DEFAULT);
  }

  ~Registry() { curl_global_cleanup(); }

  static Registry* instance() {
    static Registry* registry = nullptr;
    if (registry == nullptr) {
      registry = new Registry();
    }
    return registry;
  }

  void parse(int argc, char* argv[]) {
    if (argc < 2) {
      argparser.help();
    }
    argparser.parse(argc, argv);
  }

  template <typename T>
  static std::string& get_type_name() {
    static std::string name;

    if (name.empty()) {
      name  = __PRETTY_FUNCTION__;
      int s = name.find("with T = ");
      int e = name.find(";", s);
      name  = name.substr(s + 9, e - s - 9);
    }

    return name;
  }

  template <typename T>
  void add() {
    string name = get_type_name<T>();
    interfaces.emplace(make_pair(name.lower(), std::make_unique<T>()));
  }

  void repl() {
    while (true) {
      auto m = input("(hort) ");
      if (m == "quit" || m == "exit") { break; }
      if (m == "list") { for (const auto &[n, _] : interfaces) { print<string>(n); } }
      if (interfaces.find(m) != interfaces.end()) {
        while (true) {
          auto f = input("({}) "_format(m));
          if (f == "quit" || f == "exit") { break; }
          if (f == "archive") { interfaces[m]->archive(); }
          else if (!interfaces[m]->forward(m)) {
            std::vector<std::string> v;
            while (true) {
              auto i = input("tag > ");
              if (i == "") { break; }
              v.push_back(i);
            }
            interfaces[m]->subscribe(f, v);
          }
        }
      } else {
        forward(m);
      }
    }
  }

  void auth() {
    for (auto& [n, i] : interfaces) {
      i->auth();
    }
  }

  void forward(std::string s) {
    for (auto& [n, i] : interfaces) {
      i->forward(s);
    }
  }

  std::map<string, std::unique_ptr<Interface>> interfaces;
  Args argparser;


};

template <typename T>
struct AutoRegistry {
  AutoRegistry() {
    static bool once = false;
    if (!once) {
      Registry::instance()->add<T>();
      once = true;
    }
  }
};

} // namespace hort

#endif /* HORT_REGISTRY_HPP_ */
