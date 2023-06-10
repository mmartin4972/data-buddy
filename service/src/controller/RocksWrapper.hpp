
#include <unordered_set>
#include <memory>

#include "Common.hpp"

#include "rocksdb/db.h"
#include "rocksdb/options.h"

#ifndef RocksWrapper_hpp
#define RocksWrapper_hpp

/**
 * Wrapper class that wraps the RocksDB, so that we can constrain the 
 * way in which values are read and written to the database.
 * EFFECT: All values will be stored as json strings according to the given value schema
 * EFFECT: All keys will be stored according to the given key schema
 * NOTE: This class adds a requirement to the json schema that states that all required keys
 *      must be in the correct order and come before all other keys in the json object.
 *      This is because of how RocksDB does range searches with bloom filters.
 * NOTE: This class is not thread safe
*/
class RocksWrapper {
    rocksdb::DB* db;
    fs::path path;
    const unsigned int max_key_size = ROCKSDB_MAX_KEY_SIZE;
    const unsigned int max_value_size = ROCKSDB_MAX_VALUE_SIZE;

    public:

    // static std::unordered_set<string> opened_paths; // THIS THING IS NOT THREAD SAFE!!

    RocksWrapper(fs::path path) { // TODO: issue with this being private and being constructed by shared_ptr. Please make private again and resolve.
        if (is_path_opened(path)) {
            throw std::invalid_argument("The given path is already opened");
        };
        this->path = path;
        // RocksWrapper::opened_paths.insert(this->path.string());
    }

    bool does_json_conform_schema(const json& schema, const json& data) {
        bool result = true;
        json_schema_validator validator(nullptr, nlohmann::json_schema::default_string_format_check);
        validator.set_root_schema(schema);
        try {
            validator.validate(data);
        } catch (const std::exception& e) {
            std::cerr << "Validation failed, here is why: " << e.what() << "\n";
            result = false;
        }
        
        return result;
    }

    /**
     * Indicates if the given path is already connected to a database
     * @param path: The path to check
     * @return: True if the path is already connected to a database, false otherwise
    */
    static bool is_path_opened(fs::path path) {
        // return opened_paths.find(path.string()) != opened_paths.end();
        return false;
    }

    /**
     * Opens the database at the given path
     * @param path: The path to where the database is located
     * @return: A RocksWrapper object that wraps the database
    */
    static RocksWrapper_ptr open_db(string path) {
        RocksWrapper_ptr rocks_wrapper = std::make_shared<RocksWrapper>(fs::path(path)); // Should throw error if the path is invalid TODO: check this
        rocksdb::Options options;
        options.create_if_missing = true;
        rocksdb::Status status = rocksdb::DB::Open(options, rocks_wrapper->get_path_str(), &(rocks_wrapper->db));
        if (!status.ok()) {
            // When rocks_wrapper is destroyed it should remove from opened_paths? TODO: check this
            throw std::runtime_error("Could not open database at path: " + path);
        }
        return rocks_wrapper;
    }

    /**
     * Creates the database at the given path
     * @param path: The path to the folder to create the database at
     * @return: A RocksWrapper object that wraps the database
    */
    static RocksWrapper_ptr create_db(string path, string name) {
        fs::path new_path = fs::path(path) / fs::path(name); 
        RocksWrapper_ptr rocks_wrapper = std::make_shared<RocksWrapper>(new_path);
        rocksdb::Options options;
        options.create_if_missing = true;
        rocksdb::Status status = rocksdb::DB::Open(options, rocks_wrapper->get_path_str(), &(rocks_wrapper->db));
        if (!status.ok()) {
            throw std::runtime_error("Could not create database at path: " + path);
        }
        return rocks_wrapper;
    }

    /**
     * REQUIRES: properties that come prior to other properties cannot be empty
     * REQUIRES: the key will be constructed in the order of the required properties
     * REQUIRES: the category property is the first property
     * REQUIRES: all properties after an empty property are empty
     * REQUIRES: constructed key is within size limits
    */
    string build_query_key_from_json(const json& key_schema, const json& key) {
        string query_key = "";
        string delimiter = "::";
        // Base schema validation
        if (!does_json_conform_schema(key_schema, key)) {
            throw std::runtime_error("The given key does not conform to the given key schema");
        }

        // Check that the category is the first property
        const json& requiredKeys = json_at<json>(key_schema, "required");
        if (json_at<string>(requiredKeys, 0) != CATEGORY) {
            throw std::runtime_error("The category property must be the first property in the key schema");
        }

        // Check that properties that there are no non-empty properties after an empty property
        bool found_empty = false;
        for (const auto& req_key : requiredKeys) {
            string key_value = json_at<string>(key, req_key.get<string>()); // ERROR!! Make sure that you use the json get method here when putting in RocksWrapper
            if (found_empty && key_value != "") {
                throw std::runtime_error("The key " + req_key.dump() + " is not allowed to be non-empty when a prior key was already empty");
            } else if (key_value == "") {
                found_empty = true;
            } else {
                query_key += key_value + delimiter;
            }
        }
        query_key = query_key.substr(0, query_key.size() - delimiter.size());

        // Check that the query_key size is less than the max key size
        if (query_key.size() > max_key_size) {
            throw std::runtime_error("The constructed key is too large");
        }

        return query_key;
    }

    /**
     * Gets the value associated with the given key
     * @param key_schema: The json schema that the key must conform to
     * @param key: The key to get the value for
     * @param keys: Object that the list of keys associated with the key is returned in
     * @param values: Object that the list of value associated with the key is returned in
     * @return The value associated with the given key, or an empty string if the key does not exist
     * REQUIRES: The key is less than max_key_size
     * EFFECTS: Returns an error if the requested key does not exist
    */
    string get(const json& key_schema, const json& key, json& keys, json& values) {
        string error = "";
        keys.clear();
        values.clear();
        
        try {
            string query_key = build_query_key_from_json(key_schema, key);
            auto iter = db->NewIterator(rocksdb::ReadOptions());
            for (iter->Seek(query_key); iter->Valid() && iter->key().starts_with(query_key); iter->Next()) {
                keys.push_back(iter->key().ToString()); // TODO: may want to reconstruct json?
                values.push_back(json::parse(iter->value().ToString()));
            }
            if (values.size() == 0) {
                throw std::runtime_error("The requested key does not exist");
            }
        } catch (const std::exception& e) {
            error = e.what();
        }
        return error;
    }

    /**
     * Puts the given key-value pair into the database
     * @param key The key to put into the database
     * @param value The value to put into the database
     * @param key_schema: The json schema that the key must conform to
     * @param value_schema: The json schema that the value must conform to
     * @return An empty string if the put was successful, otherwise an error message
     * REQUIRES: The key is less than max_key_size
     * REQUIRES: The value is less than max_value_size
    */
   // TODO: you may want to consider throwing an error instead of returning a string?
    string put(const json& key_schema, const json& key, const json& value_schema, const json& value) {
        string error = "";
        if (key.size() > max_key_size) {
            error = "The given key is too large";
        } else if (value.size() > max_value_size) {
            error = "The given value is too large";
        } else if (!does_json_conform_schema(key_schema, key)) {
            error = "The given key does not conform to the given schema";
        } else if (!does_json_conform_schema(value_schema, value)) {
            error = "The given value does not conform to the given schema";
        } else {
            string query_key = build_query_key_from_json(key_schema, key);
            rocksdb::Status status = db->Put(rocksdb::WriteOptions(), query_key, value.dump());
            if (!status.ok()) {
                error += status.ToString();
            }
        }
        return error;
    }

    /**
     * Deletes the given key-value pair from the database
     * @param key The key to delete from the database
     * @param key_schema: The json schema that the key must conform to
     * @return An empty string if the delete was successful, otherwise an error message
     * REQUIRES: The key is less than max_key_size
    */
    string del(const json& key_schema, const json& key) {
        string error = "";
        if (key.size() > max_key_size) {
            error = "The given key is too large";
        } else if (!does_json_conform_schema(key_schema, key)) {
            error = "The given key does not conform to the given schema";
        } else {
            string query_key = build_query_key_from_json(key_schema, key);
            rocksdb::Status status = db->Delete(rocksdb::WriteOptions(), query_key);
            if (!status.ok()) {
                error += status.ToString();
            }
        }
        return error;
    }

    /**
     * Indicates if the wrapper is connected to the database i.e. has a valid database pointer
     * @return True if the wrapper is connected to the database, false otherwise
    */
    bool is_connected() {
        return db != nullptr;
    }
    // TODO: May be able to get rid of this function

    /**
     * Clean things up in the destructor
     * NOTE: we don't want to make this private as then we wouldn't be able to call it from user code
    */
    ~RocksWrapper() {
        // opened_paths.erase(path.string());
        delete db;
    }

    string get_path_str() {
        return path.string();
    }
};

#endif // RocksWrapper_hpp
