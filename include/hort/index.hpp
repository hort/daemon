#ifndef MONGOCLIENT_HPP_
#define MONGOCLIENT_HPP_

#include "json.hpp"
using json = nlohmann::json;

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/exception/exception.hpp>

#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

namespace hort
{

class Index
{

	static mongocxx::database &create_database()
	{
		static mongocxx::instance instance{};
		static auto uri      = mongocxx::uri("mongodb://localhost:27017");
		static auto client   = mongocxx::client(uri);
		static auto database = client["hortdb"];
		return database;
	}

	mongocxx::database &database = create_database();
	mongocxx::collection collection;

public:
	Index(const std::string &coll);

	/// \brief Inserts a document inside the index
	void insert(json d);

	/// \brief Queries index for all documents
	json query();

	/// \brief Update field with new_field of document with key equal to value
	template <typename T>
	void update(const std::string &key, const std::string &value, const std::string &field, const T &new_value)
	{
		collection.update_one(
				document{} << key    << value         << finalize,
				document{} << "$set" << open_document << field << new_value << close_document<< finalize);
	}

};

} // namespace hort

#endif /* MONGOCLIENT_HPP_ */
