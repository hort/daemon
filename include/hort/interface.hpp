#ifndef INTERFACE_H
#define INTERFACE_H

#include <loki/builder.hpp>

// #include "registry.hpp" <- imported at the end of the file

#include "hort/archive.hpp"
#include "hort/chrono.hpp"
#include "hort/filesystem.hpp"
#include "hort/http.hpp"
#include "hort/index.hpp"
#include "hort/print.hpp"
#include "hort/random.hpp"
#include "hort/string.hpp"
#include "hort/worker.hpp"

#define HORT_ENABLE_DEPRECATED
#define HORT_ENABLE_NODISCARD

#ifdef __has_cpp_attribute
#  if __has_cpp_attribute(deprecated) and defined(HORT_ENABLE_DEPRECATED)
#    if __cplusplus >= 201309L
#      define HORT_DEPRECATED(message) [[deprecated(message)]]
#    else
#      define HORT_DEPRECATED(message) __attribute__((deprecated(message)))
#    endif
#  endif
#endif
#ifndef HORT_DEPRECATED
#  define HORT_DEPRECATED(message)
#endif

#ifdef __has_cpp_attribute
#  if __has_cpp_attribute(nodiscard) and defined(HORT_ENABLE_NODISCARD)
#    if __cplusplus >= 201309L
#      define HORT_NODISCARD [[nodiscard]]
#    else
#      define HORT_NODISCARD __attribute__((nodiscard))
#    endif
#  endif
#endif
#ifndef HORT_NODISCARD
#  define HORT_NODISCARD
#endif

/// \brief Define a hort interface. This will automatically register the class
/// to the global registry which will automatically handle everything.
///
/// \code
/// HORT_INTERFACE(Api) {
/// 	Api(args...) : hort::Interface("api",
/// 	{ { regex, callback }
/// 	, { regex, callback }
/// 	}) ;
///
/// 	void auth() override;
/// }
/// \endcode
///
/// \see hort::AutoRegistry
#define HORT_INTERFACE(T)                                                     \
class T;                                                                      \
static hort::AutoRegistry<T> _##T;                                            \
class T : public hort::Interface

HORT_DEPRECATED("Use hort::chrono::sleep instead")
inline void sleep(int milliseconds)
{
	std::chrono::duration<int, std::milli> timespan(milliseconds);
	std::this_thread::sleep_for(timespan);
}

namespace hort
{

class Interface
{

	using rules_t = std::vector<std::pair<std::string, std::function<void(const re::Match&)>>>;

	static loki::Registry &create_registry()
	{
		static loki::Registry builder = loki::Builder{}
					.LogLevel(loki::Level::Debug)
					.PrintLevel(loki::Level::Debug)
					.Colorize(loki::Level::Warn, loki::TermColor::Yellow)
					.Colorize(loki::Level::Error, loki::TermColor::Red)
					.FlushProtocol(loki::Protocol::Protobuf)
					.FlushInterval(10000)
					.MaxBuffer(1000)
					.Labels({{"app", "hortd"}})
					.Build();
		return builder;
	}

	loki::Registry &registry = create_registry();

	std::string serialize()
	{
		return state.dump();
	}

	void unserialize(const std::string &s)
	{
		state = json::parse(s);
	}

	std::vector<std::pair<re::Regex, std::function<void(const re::Match&)>>> rules;

protected:
	http::Session session;
	loki::Agent  &logger;
	Index         index;

	const std::string id;
	const std::string hortpath;

	json state;

public:
	Interface(const std::string &id_, rules_t &&rules_ = {})
		: session{"/home/mirco/.hort/" + id_ + "/"+ "cookies.txt"}
		, logger{registry.Add({{"interface", id_}})}
		, index{id_ + "_subscriptions"}
		, id{id_}
		, hortpath{"/home/mirco/.hort/" + id_ + "/"}
		, state{}
	{
		for (const auto &[rule, callback] : rules_) {
			rules.emplace_back(std::make_pair(re::Regex{rule}, std::move(callback)));
		}
	}

	virtual void auth() { };

	/// \brief Wrapper function for user defined auth.
	void _auth()
	{
		logger.Infof("authenticating {} interface", id);
		auth();
	}

	/// \brief Check if s matches any rule, if it does, permorm callback.
	void forward(const std::string &s)
	{
		for (auto &[rule, callback] : rules) {
			if (auto match = rule.findall(s)) {
				callback(match);
			}
		}
	}

};

} // namespace hort

#include "registry.hpp"

#endif /* INTERFACE_H */
