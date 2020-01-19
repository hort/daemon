#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_

#include "hort/formats.hpp"
#include "hort/re.hpp"

namespace hort::http {

struct Response
{

	std::string body;
	std::string url;
	std::map<std::string, std::string> headers;
	long code;

	json parse()
	{
		try {
			return json::parse(body);
		} catch(const json::basic_json::parse_error &e) {
			return nullptr;
		}
	};

	json feedparse()
	{
		return hort::formats::xml2json(body.c_str());
	}

	json findall(const std::string &pattern)
	{
		return hort::re::findall(pattern, body);
	}

};

} // namespace hort::http

#endif /* RESPONSE_HPP_ */
