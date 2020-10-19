#ifndef __HORT_INTERFACE_HPP_
#define __HORT_INTERFACE_HPP_

#include <loki/builder.hpp>

/// #include "hort/registry.hpp"

#include "hort/archive.hpp"
#include "hort/chrono.hpp"
#include "hort/filesystem.hpp"
#include "hort/formats.hpp"
#include "hort/http.hpp"
#include "hort/index.hpp"
#include "hort/print.hpp"
#include "hort/random.hpp"
#include "hort/range.hpp"
#include "hort/re.hpp"
#include "hort/string.hpp"
#include "hort/worker.hpp"

/// \brief Define a hort interface. This will automatically register the class
/// to the global registry which will automatically handle everything.
///
/// \code
/// HORT_INTERFACE(Api) {
/// public:
///   Api() : hort::Interface("api",
///   { { regex, CB({ expression }) },
///   , { regex, CB({ expression }) },
///   } {}
///
/// public:
///   void auth() override {}
///
/// private:
///   void foobar() {}
/// };
/// \endcode
///
/// \see hort::AutoRegistry
#define HORT_INTERFACE(T)                                                      \
  class T;                                                                     \
  static hort::AutoRegistry<T> T##_;                                           \
  class T final : public hort::Interface

#define CB(callback) \
  ([this](const hort::vector<hort::string> &v) { callback })

#define HORT_RULE_CALLBACK(regex, callback)                                    \
  { regex, [this](const hort::vector<hort::string> &v) { callback } },

namespace hort {

class Interface {

  using rules_type = std::vector<std::pair<std::string, std::function<void(const vector<string>&)>>>;
  using compiled_rules_type = std::vector<std::pair<re::regex, std::function<void(const vector<string>&)>>>;

  static loki::Registry<loki::AgentJson> &create_registry() {
    static loki::Registry builder =
        loki::Builder<loki::AgentJson>{}
            .LogLevel(loki::Level::Info)
            .PrintLevel(loki::Level::Info)
            .Colorize(loki::Level::Warn, loki::Color::Yellow)
            .Colorize(loki::Level::Error, loki::Color::Red)
            .FlushInterval(10000)
            .MaxBuffer(1000)
            .Labels({{"app", "hortd"}})
            .Build();
    return builder;
  }

  loki::Registry<loki::AgentJson> &registry = create_registry();

  [[nodiscard]] std::string serialize() const {
    return state.dump();
  }

  void unserialize(json&& s) {
    state = s;
  }

public:
  const std::string id;
  const std::string hortpath;

  compiled_rules_type rules{};

protected:
  http::Session session;
  loki::Agent& logger;
  Index index;

  json state{};

public:
  Interface(const std::string& id_, rules_type&& rules_ = {})
      : id{id_}
      , hortpath{"/home/mirco/.hort/" + id_ + "/"}
      , session{hortpath + "cookies.txt"}
      , logger{registry.Add({{"interface", id_}})}
      , index{id_ + "_subscriptions"} {
    for (const auto& [ rule, callback ] : rules_) {
      rules.emplace_back(std::make_pair(re::regex{rule}, callback));
    }
    unserialize(formats::loadyaml(hortpath + "config.yml"));
  }

  virtual ~Interface() {
    filesystem::write(serialize(), hortpath, "config.yml");
  }

  virtual void auth() {};
  virtual void subscribe([[maybe_unused]] const std::string &username,
                         [[maybe_unused]] const std::vector<std::string> &tags) {}
  virtual void archive() {};

  /// \brief Wrapper function to archive the index entries.
  void _archive() {
    logger.Infof("authenticating to interface");
    auth();
    logger.Infof("started to archive interface {}", id);
    archive();
    logger.Infof("finishsed to archive interface {}", id);
  }

  /// \brief Wrapper function for user defined auth.
  void _auth() {
    auth();
  }

  /// \brief Check if s matches any rule, if it does, permorm callback.
  bool forward(const std::string& s) {
    for (auto& [ rule, callback ] : rules) {
      if (auto match = rule.findall(s)) {
        logger.Infof("matched {} with interface {}", s, id);
        callback(match);
        return true;
      }
    }
    return false;
  }

  [[nodiscard]] std::string dump() {
    return index.query().dump(1);
  }

};

} // namespace hort

#include "registry.hpp"

#endif // __HORT_INTERFACE_HPP_
