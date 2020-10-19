#include "hort/registry.hpp"

namespace hort {

void Registry::parse(int argc, char* argv[]) {
  if (argc < 2) {
    argparser.help();
  }
  argparser.parse(argc, argv);
}

void Registry::auth() {
  for (auto& [n, i] : interfaces) {
    i->auth();
  }
}

void Registry::forward(const std::string &s) {
  for (auto& [n, i] : interfaces) {
    i->forward(s);
  }
}

void Registry::repl() {
  while (true) {
    auto m = input("(hort) ").trim();
    if (m == "quit" || m == "exit") { break; }
    if (m == "list") { for (const auto &[n, _] : interfaces) { print(n); } }
    if (interfaces.find(m) != interfaces.end()) {
      while (true) {
        auto f = input(hort::sprintf("({}) ", m)).trim();
        if (f == "quit" || f == "exit") { break; }
        if (f == "archive") { interfaces[m]->_archive(); break; }
        if (f == "rules") {
          for (const auto &[regex, _] : interfaces[m]->rules) {
            print(regex.pattern);
          }
        }
        else if (!interfaces[m]->forward(f.trim())) {
          std::vector<std::string> v;
          while (true) {
            auto i = input("tag > ").trim();
            if (i == "") break;
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

} // namespace hort
