#ifndef HORT_INDEX_HPP_
#define HORT_INDEX_HPP_

#include "json.hpp"
using json = nlohmann::json;

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/instance.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

namespace hort {

/// TODO: check for connection status
class Index {

  static mongocxx::database& create_database() {
    static mongocxx::instance instance{};
    static auto uri      = mongocxx::uri("mongodb://localhost:27017");
    static auto client   = mongocxx::client(uri);
    static auto database = client["hortdb"];
    return database;
  }

  mongocxx::database& database = create_database();
  mongocxx::collection collection;

public:
  explicit Index(const std::string& coll) : collection{database[coll]} {}

  /// \brief Insert a document inside the index.
  int insert(json d) {
    try {
      auto result = collection.insert_one(bsoncxx::from_json(d.dump()));
    } catch (const mongocxx::exception& e) { return e.code().value(); }
    return 0;
  }

  /// \brief Query index for all documents.
  [[nodiscard]] json query() {
    json docs;
    try {
      auto cursor = collection.find({});
      for (auto&& doc : cursor) {
        docs.emplace_back(json::parse(bsoncxx::to_json(doc)));
      }
    } catch (const mongocxx::exception& e) {
      return json{{"error", e.code().value()}};
    }
    return docs;
  }

  /// \brief Query index for all documents.
  [[nodiscard]] json query(bsoncxx::document::view_or_value filter) {
    json docs;
    try {
      auto cursor = collection.find({filter});
      for (auto&& doc : cursor) {
        docs.emplace_back(json::parse(bsoncxx::to_json(doc)));
      }
    } catch (const mongocxx::exception& e) {
      return json{{"error", e.code().value()}};
    }
    return docs;
  }

  int update(bsoncxx::document::view_or_value filter,
             bsoncxx::document::view_or_value update) {
    try {
      collection.update_one(filter, update);
    } catch (const mongocxx::exception& e) { return e.code().value(); }
    return 0;
  }

  template <typename FilterType, typename UpdateType>
  int update(const std::string& filter_key,
             const FilterType& filter_value,
             const std::string& update_key,
             const UpdateType& update_value) {
    try {
      collection.update_one(
          document{} << filter_key << filter_value << finalize,
          document{} << "$set" << open_document << update_key << update_value
                     << close_document << finalize);
    } catch (const mongocxx::exception& e) { return e.code().value(); }
    return 0;
  }
};

} // namespace hort

#endif /* HORT_INDEX_HPP_ */
