#ifndef HORT_FORMATS_HPP_
#define HORT_FORMATS_HPP_

#include <rapidxml.hpp>
#include <yaml-cpp/yaml.h>

#include "json.hpp"
using json = nlohmann::json;

namespace hort::formats {

namespace detail {

json xml2json(rapidxml::xml_node<>* root);
json yaml2json(YAML::Node root);

} // namespace detail

/// \brief Convert an xml encoded string into a json object
[[nodiscard]] json xml2json(const std::string& str);

/// \brief Convert an yaml encoded string into a json object
[[nodiscard]] json yaml2json(const std::string& str);

/// \brief Load a file containing a xml document into a json
[[nodiscard]] json loadxml(const std::string& filepath);

/// \brief Load a file containing a yaml document into a nlohmann::json object.
[[nodiscard]] json loadyaml(const std::string& filepath);

} // namespace hort::formats

#endif /* HORT_FORMATS_HPP_ */
