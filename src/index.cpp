#include "index.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace hort
{

Index::Index(const std::string &coll)
	: collection{database[coll]} {}

void Index::insert(json d)
{
	auto result = collection.insert_one(bsoncxx::from_json(d.dump()));
}

json Index::query()
{
	json docs;
	auto cursor = collection.find({});
	for (auto &&doc : cursor)
		docs.emplace_back(json::parse(bsoncxx::to_json(doc)));
	return docs;
}

} // namespace hort

