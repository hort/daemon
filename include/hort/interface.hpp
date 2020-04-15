#ifndef HORT_INTERFACE_HPP_
#define HORT_INTERFACE_HPP_

#include <loki/builder.hpp>

/// Due to a complex circular dependency I had to import the file "registry.hpp" at the bottom.

#include "hort/archive.hpp"
#include "hort/chrono.hpp"
#include "hort/filesystem.hpp"
#include "hort/http.hpp"
#include "hort/index.hpp"
#include "hort/print.hpp"
#include "hort/random.hpp"
#include "hort/range.hpp"
#include "hort/regexp.hpp"
#include "hort/string.hpp"
#include "hort/worker.hpp"

/// \brief Define a hort interface. This will automatically register the class
/// to the global registry which will automatically handle everything.
///
/// \code
/// HORT_INTERFACE(Api) {
///   Api(args...) : hort::Interface("api",
///   { { regex, callback }
///   , { regex, callback }
///   } {}
///
///   void auth() override {}
/// }
/// \endcode
///
/// \see hort::AutoRegistry
#define HORT_INTERFACE(T)                                                      \
  class T;                                                                     \
  static hort::AutoRegistry<T> T##_;                                           \
  class T final : public hort::Interface

#define HORT_RULE_CALLBACK(regex, callback)                                    \
  { regex, [this](const hort::vector<hort::string> &v) { callback } },

namespace hort {

class Interface {

  using rules_t = std::vector<
      std::pair<std::string, std::function<void(const vector<string>&)>>>;

  /*
  static loki::Registry<loki::AgentJson>& create_registry() {
    static loki::Registry builder =
        loki::Builder<loki::AgentJson>{}
            .LogLevel(loki::Level::Debug)
            .PrintLevel(loki::Level::Debug)
            .Colorize(loki::Level::Warn, loki::Color::Yellow)
            .Colorize(loki::Level::Error, loki::Color::Red)
            .FlushInterval(10000)
            .MaxBuffer(1000)
            .Labels({{"app", "hortd"}})
            .Build();
    return builder;
  }

  loki::Registry<loki::AgentJson>& registry = create_registry();
  */

  std::string serialize() const {
    return state.dump();
  }

  void unserialize(const std::string& s) {
    state = json::parse(s);
  }

  std::vector<std::pair<regexp::regex, std::function<void(const vector<string>&)>>> rules{};

protected:
  http::Session session;
  /*
  loki::Agent& logger;
  */
  Index index;

  const std::string id;
  const std::string hortpath;

  json state{};

public:
  Interface(const std::string& id_, rules_t&& rules_ = {})
      : session{"/home/mirco/.hort/" + id_ + "/" + "cookies.txt"}
      /*
      , logger{registry.Add({{"interface", id_}})}
      */
      , index{id_ + "_subscriptions"}
      , id{id_}
      , hortpath{"/home/mirco/.hort/" + id_ + "/"} {
    for (const auto& [ rule, callback ] : rules_) {
      rules.emplace_back(std::make_pair(regexp::regex{rule}, callback));
    }
  }

  virtual ~Interface() {  }

  virtual void auth() {};
  virtual void subscribe([[maybe_unused]] const std::string &username, [[maybe_unused]] const std::vector<std::string> &tags) {}
  virtual void archive() {}

  /// \brief Wrapper function for user defined auth.
  void _auth() {
    auth();
  }

  /// \brief Check if s matches any rule, if it does, permorm callback.
  bool forward(const std::string& s) {
    for (auto& [ rule, callback ] : rules) {
      if (auto match = rule.findall(s)) {
        hort::print(":: mathed interface {}"_format(id));
        callback(match);
        return true;
      }
    }
    return false;
  }
};

} // namespace hort

#include "registry.hpp"

#endif /* HORT_INTERFACE_HPP_ */
