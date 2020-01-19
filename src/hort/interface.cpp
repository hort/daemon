#include "hort/interface.hpp"

namespace hort
{

Interface::Interface(const std::string &id_, rules_t &&rules_)
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

Interface::~Interface()
{
}

void Interface::forward(const std::string &s)
{
	for (auto &[rule, callback] : rules) {
		if (auto match = rule.findall(s)) {
			callback(match);
		}
	}
}

void Interface::_auth()
{
	logger.Infof("authenticating {} interface", id);
	auth();
}

std::string Interface::serialize()
{
	return state.dump();
}

void Interface::unserialize(const std::string &s)
{
	state = json::parse(s);
}

} // namespace hort
