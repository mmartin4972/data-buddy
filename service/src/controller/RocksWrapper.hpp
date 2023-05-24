#include <unordered_set>
#include <memory>

#include "Common.hpp"

#include "rocksdb/db.h"
#include "rocksdb/options.h"

/**
 * Wrapper class that wraps the RocksDB, so that we can constrain the 
 * way in which values are read and written to the database.
 * EFFECT: All values will be stored as json strings
 * EFFECT: All keys will be of the form: category:<identifier>
*/
class RocksWrapper {
    private:
    static unordered_set<fs::path> opened_paths;
    rocksdb::DB* db;
    fs::path path;

    RocksWrapper(fs::path path) {
        if (is_path_opened(path)) {
            throw std::invalid_argument("The given path is already opened");
        };
        this->path = path;
        opened_paths.insert(this->path);
    }

    public:

    /**
     * Indicates if the given path is already connected to a database
     * @param path: The path to check
     * @return: True if the path is already connected to a database, false otherwise
    */
    static bool is_path_opened(fs::path path) {
        return paths.find(path) != paths.end();
    }

    /**
     * Opens the database at the given path
     * @param path: The path to where the database is located
     * @return: A RocksWrapper object that wraps the database
    */
    static Common::RocksWrapper_ptr open_db(std::string path) {
        Common::RocksWrapper_ptr rocks_wrapper = std::make_shared<RocksWrapper>(fs::path(path)); // Should throw error if the path is invalid TODO: check this
        rocksdb::Options options;
        options.create_if_missing = true;
        rocksdb::Status status = rocksdb::DB::Open(options, rocks_wrapper.get_path_str(), &rocks_wrapper.db);
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
    static Common::RocksWrapper_ptr create_db(std::string path, std::string name) {
        fs::path new_path = fs::path(path) / fs::path(name); 
        Common::RocksWrapper_ptr rocks_wrapper = std::make_shared<RocksWrapper>(new_path);
        rocksdb::Options options;
        options.create_if_missing = true;
        rocksdb::Status status = rocksdb::DB::Open(options, rocks_wrapper.get_path_str(), &rocks_wrapper.db);
        if (!status.ok()) {
            throw std::runtime_error("Could not create database at path: " + path);
        }
        return rocks_wrapper;
    }

    /**
     * Gets the value associated with the given key
     * @param key The key to get the value for
     * @return The value associated with the given key, or an empty string if the key does not exist
    */
    std::string get(const std::string& key, Dictionary& value) {
        std::string error = "";
        std::string tmp;
        rocksdb::Status status = db->Get(rocksdb::ReadOptions(), key, &tmp);
        if (!status.ok()) {
            error = status.ToString();
        } else {
            value = Common::string_to_dictionary(tmp);
        }
        return error;
    }

    /**
     * Puts the given key-value pair into the database
     * @param key The key to put into the database
     * @param value The value to put into the database
     * @return An empty string if the put was successful, otherwise an error message
    */
    std::string put(const std::string& key, Dictionary& value) {
        std::string error = "";
        rocksdb::Status status = db->Put(rocksdb::WriteOptions(), key, (std::string)Common::dictionary_to_string(value));
        if (!status.ok()) {
            error = status.ToString();
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
        opened_paths.erase(path);
        delete db;
    }

    std::string get_path_str() {
        return path.string();
    }
}