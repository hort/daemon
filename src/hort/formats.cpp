#include "hort/formats.hpp"

#include <fstream>

namespace hort::formats
{

namespace detail
{

json xml2json(rapidxml::xml_node<> *root)
{
	json j{};
	std::map<std::string, int> occurrence{};

	for (auto *node = root->first_node(); node; node = node->next_sibling()) {
		const char *key = node->name();
		occurrence[key] += 1;

		// if already seen, convert it into a list
		if (occurrence[key] == 2)
			j[key] = { j[key] };

		// if the node is a sequence
		if (occurrence[key] > 1) {
			// create a new json object and push it into the vector
			json n{};
			if(node->first_node())
				n[key] = xml2json(node);
			else
				n["#text"] = node->value();
			// iterate through the attributes
			for (auto *attr = node->first_attribute(); attr; attr = attr->next_attribute())
				n[key][attr->name()] = attr->value();
			j[key].emplace_back(n[key]);
		} else {
			if (node->first_node()) {
				j[key] = xml2json(node);
			} else {
				j["#text"] = node->value();
			}
			// iterate through the attributes
			for (auto *attr = node->first_attribute(); attr; attr = attr->next_attribute())
				j[key][attr->name()] = attr->value();
		}
	}

	return j;
}

json yaml2json(YAML::Node root)
{
	json j{};
	switch (root.Type()) {
	case YAML::NodeType::Null:
		break;
	case YAML::NodeType::Scalar:
		return root.as<std::string>();
	case YAML::NodeType::Sequence:
		for (auto node : root)
			j.emplace_back(yaml2json(node));
		break;
	case YAML::NodeType::Map:
		for (const auto &it : root) {
			YAML::Node key   = it.first;
			YAML::Node value = it.second;
			j[key.as<std::string>()] = yaml2json(value);
		}
		break;
	default:
		break;
	}
	return j;
}

} // namespace detail

json xml2json(const std::string &str)
{
	json j{};
	rapidxml::xml_document<> doc;
	try {
		// might be somewhat dangerous
		doc.parse<0>(const_cast<char*>(str.data()));
	} catch (const rapidxml::parse_error &e) {
		return nullptr;
	} catch (const std::exception &e) {
		return nullptr;
	}
	rapidxml::xml_node<> *root = doc.first_node();
	if (root)
		j[root->name()] = detail::xml2json(root);
	return j;
}


json yaml2json(const std::string &str)
{
	YAML::Node root;
	try {
			root = YAML::Load(str);
	} catch (const YAML::ParserException &e) {
		return nullptr;
	} catch (const std::exception &e) {
		return nullptr;
	}
	return detail::yaml2json(root);
}

json loadyaml(const std::string &filepath)
{
	YAML::Node root;
	try {
		root = YAML::LoadFile(filepath);
	} catch (const YAML::ParserException &e) {
		return nullptr;
	} catch (const std::exception& e) {
		return nullptr;
	}
	return detail::yaml2json(root);
}

json loadxml(const std::string &filepath)
{
	std::ifstream file(filepath);
	std::string str((
		std::istream_iterator<char>(file)),
		std::istream_iterator<char>());
	return xml2json(str);
}

} // namespace hort::formats
