#ifndef INTERFACE_H
#define INTERFACE_H

#include <loki/builder.hpp>

// #include "registry.hpp" <- imported at the end of the file

#include "hort/archive.hpp"
#include "hort/filesystem.hpp"
#include "hort/http/form.hpp"
#include "hort/http/session.hpp"
#include "hort/index.hpp"

/// \brief Define a class inheriting from Interface and automatically register
/// it to the registry.
///
/// If the derived class implements methods such as `auth` they will be
/// automatically called. The state of the instance can be stored in the `state`
/// object which will automatically be unserialized and serialized at the
/// corresponding object construction and destruction.
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
#define HORT_INTERFACE(T)                                                     \
class T;                                                                      \
static hort::AutoRegistry<T> _##T;                                            \
class T : public hort::Interface

/// \todo Move to appropriate file.
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

	std::string serialize();
	void unserialize(const std::string &s);

	std::vector<std::pair<re::Regex, std::function<void(const re::Match&)>>> rules;

protected:
	http::Session session;
	loki::Agent  &logger;
	Index         index;

	const std::string id;
	const std::string hortpath;

	json state;

public:
	Interface(const std::string &id_, rules_t &&rules_ = {});
	~Interface();

	virtual void auth() { };

	/// \brief Wrapper function for user defined auth.
	void _auth();

	/// \brief Check if s matches any rule, if it does, permorm callback.
	void forward(const std::string &s);

};

} // namespace hort

#include "registry.hpp"

#endif /* INTERFACE_H */
