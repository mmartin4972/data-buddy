#include "controller/Controller.hpp"
#include <iostream>
#include <string>
#include "rocksdb/db.h"
#include "rocksdb/options.h"

int Controller::db_test() {
    rocksdb::DB* db;
    rocksdb::Options options;
    options.create_if_missing = true;

    // Open the database
    rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/testdb", &db);
    if (!status.ok()) {
        std::cerr << "Error opening database: " << status.ToString() << std::endl;
        return 1;
    }

    // Write a key-value pair
    std::string key = "hello";
    std::string value = "world";
    status = db->Put(rocksdb::WriteOptions(), key, value);
    if (!status.ok()) {
        std::cerr << "Error writing to database: " << status.ToString() << std::endl;
        return 1;
    }

    // Read the value back
    std::string result;
    status = db->Get(rocksdb::ReadOptions(), key, &result);
    if (!status.ok()) {
        std::cerr << "Error reading from database: " << status.ToString() << std::endl;
        return 1;
    }
    std::cout << "Value for key " << key << ": " << result << std::endl;

    // Close the database
    delete db;

    return 0;
}

Dictionary Controller::string_to_dictionary(String str) {
    return Dictionary();
}

String Controller::dictionary_to_string(Dictionary dict) {
    return String("Hello World!");
}

bool Controller::is_dictionary_valid(Dictionary dict, String category) {
    return true;
}

String Controller::do_get(String auth_token, String group, String category, Dictionary key_params, StringVector& value_params) {
    return {"Hellow", {"Hello", "World!"}};
}

String Controller::do_put(String auth_token, String group, String category, Dictionary key_params, Dictionary value_params) {
    return "";
}

String do_create_buddy(String path, String& folder_path) {
    return "";
}

String do_connect_buddy(String path) {
    return "";
}

String do_create_client(String name, String password, String& auth_token) {
    return "";
}

String do_connect_client(String name, String password, String& auth_token) {
    return "";
}

String do_add_client(String auth_token, String group, String name) {
    return "";
}

String do_create_group(String auth_token, String category, String group_name) {
    return "";
}

String do_create_category(String name, StringVector key_params, StringVector value_params) {
    return "";
}

String do_list_clients() {
    return "";
}

String do_list_groups() {
    return "";
}

String do_list_categories() {
    return "";
}












