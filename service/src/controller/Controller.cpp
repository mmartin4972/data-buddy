#include "controller/Controller.hpp"
#include <iostream>
#include <string>
#include "rocksdb/db.h"
#include "rocksdb/options.h"
#include <nlohmann/json.hpp>
#include <filesystem>

using json = nlohmann::json;

////////////////////////
//
//  Helper Functions
//
////////////////////////

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

Dictionary Controller::string_to_dictionary(String jsonString) {
    Dictionary dict;
    json jsonData = json::parse(jsonString);

    for (json::iterator it = jsonData.begin(); it != jsonData.end(); ++it) {
        if (it.value().is_string()) {
            dict[it.key()] = it.value().get<std::string>();
        }
    }

    return map;
}

String Controller::dictionary_to_string(Dictionary dict) {
    json jsonMap;

    for (const auto& pair : dict) {
        jsonMap[pair.first] = pair.second;
    }

    return jsonMap.dump();
}

bool Controller::is_dictionary_valid(Dictionary dict, String category) {
    return true;
}

bool Controller::is_successful(String error) {
    return error == "";
}

Response Controller::create_response(const oatpp::Void &dto) {
    if (is_successful(dto->error)) {
        res = createDtoResponse(Status::CODE_200, dto);
    } else {
        res = createDtoResponse(Status::CODE_500, dto);
    }
    return res;
}


////////////////////////
//
//  Endpoint Functions
//
////////////////////////

String Controller::do_get(String auth_token, String group, String category, Dictionary key_params, StringVector& value_params) {
    value_params = {"Hello", "World!"};
    return "";
}

String Controller::do_put(String auth_token, String group, String category, Dictionary key_params, Dictionary value_params) {
    return "";
}

// TODO: Security vulnerability in that anyone can read the app data, which should be private
String Controller::do_create_buddy(String path, String& folder_path) {
    String error = "";
    filesystem::path p = filesystem::path(path);
    p.append(DATA_BUDDY_FOLDER)
    try {
        // Create the folder to house all data buddy files
        create_directories(p);
        buddy_path = p;
        folder_path = p.string();

        // Create the app database
        filesystem::path app_path = p;
        app_path.append(APP_DB);
        rocksdb::Options options;
        options.create_if_missing = true;
        rocksdb::Status status = rocksdb::DB::Open(options, app_path.string(), app_db);
        if (!status.ok()) {
            throw std::runtime_error(status.ToString());
        }

        // Create the user database
        filesystem::path user_path = p;
        app_path.append(USER_DB);
        rocksdb::Options options;
        options.create_if_missing = true;
        rocksdb::Status status = rocksdb::DB::Open(options, app_path.string(), user_db);
        if (!status.ok()) {
            throw std::runtime_error(status.ToString());
        }

    } catch (const filesystem_error& e) {
        error = e.what();
    } catch (const std::runtime_error& e) {
        error = e.what();
    }

    return error;
}

String Controller::do_connect_buddy(String path) {
    string error = "";
    buddy_path = filesystem::path(path);
    try {
        if (!filesystem::exists(buddy_path)) {
            throw std::runtime_error("Buddy path does not exist");
        }

        // Open the app database
        filesystem::path app_path = buddy_path;
        app_path.append(APP_DB);
        rocksdb::Options options;
        rocksdb::Status status = rocksdb::DB::Open(options, app_path.string(), app_db);
        if (!status.ok()) {
            throw std::runtime_error(status.ToString());
        }

        // Open the user database
        filesystem::path user_path = buddy_path;
        user_path.append(USER_DB);
        rocksdb::Options options;
        rocksdb::Status status = rocksdb::DB::Open(options, user_path.string(), user_db);
        if (!status.ok()) {
            throw std::runtime_error(status.ToString());
        }

    } catch (const filesystem_error& e) {
        error = e.what();
    } catch (const std::runtime_error& e) {
        error = e.what();
    }

    return error;
}

String Controller::do_create_client(String name, String password, String& auth_token) {
    return "";
}

String Controller::do_connect_client(String name, String password, String& auth_token) {
    return "";
}

String Controller::do_add_client(String auth_token, String group, String name) {
    return "";
}

String Controller::do_create_group(String auth_token, String category, String group_name) {
    return "";
}

String Controller::do_create_category(String name, StringVector key_params, StringVector value_params) {
    return "";
}

String Controller::do_list_clients() {
    return "";
}

String Controller::do_list_groups() {
    return "";
}

String Controller::do_list_categories() {
    return "";
}












