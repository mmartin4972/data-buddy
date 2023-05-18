#include "controller/Controller.hpp"
#include <iostream>
#include <string>
#include "rocksdb/db.h"
#include "rocksdb/options.h"
#include <nlohmann/json.hpp>
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;

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

Dictionary Controller::string_to_dictionary(String string) {
    Dictionary dict;
    std::string jsonString = (std::string)string;
    json jsonData = json::parse(jsonString);

    for (json::iterator it = jsonData.begin(); it != jsonData.end(); ++it) {
        if (it.value().is_string()) {
            dict[it.key()] = it.value().get<std::string>();
        }
    }

    return dict;
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

Response Controller::create_response(const String& error, const oatpp::Void &dto) {
    Response res;
    if (is_successful(error)) {
        res = createDtoResponse(Status::CODE_200, dto);
    } else {
        res = createDtoResponse(Status::CODE_500, dto);
    }
    return res;
}

void open_db_or_error(const rocksdb::Options &options, const std::string &name, rocksdb::DB **dbptr) {
    rocksdb::Status status = rocksdb::DB::Open(options, name, dbptr);
    if (!status.ok()) {
        throw std::runtime_error(status.ToString());
    }
}

bool Controller::is_buddy_connected() {
    return buddy_path.empty() == false;
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
    std::string error = "";
    rocksdb::Options options;
    options.create_if_missing = true;
    fs::path p(path);
    p.append(DATA_BUDDY_FOLDER);
    try {
        if (is_buddy_connected()) {
            throw std::runtime_error("Buddy path is already connected");
        }
        // Create the folder to house all data buddy files
        create_directory(p);
        buddy_path = p;
        folder_path = p.string();

        // Create the app database
        fs::path app_path = p;
        app_path.append(APP_DB);
        open_db_or_error(options, app_path.string(), &app_db);

        // Create the user database
        fs::path user_path = p;
        user_path.append(USER_DB);
        open_db_or_error(options, user_path.string(), &user_db);

    } catch (const fs::filesystem_error& e) {
        error = e.what();
        error += (std::string)do_disconnect_buddy();

    } catch (const std::runtime_error& e) {
        error = e.what();
        error += (std::string)do_disconnect_buddy();
    }

    return error;
}

String Controller::do_connect_buddy(String path) {
    std::string error = "";
    rocksdb::Options options;
    try {
        if (is_buddy_connected()) {
            throw std::runtime_error("Buddy path is already connected");
        }
        if (!fs::exists(fs::path(path))) {
            throw std::runtime_error("Buddy path does not exist");
        }

        // Set up buddy path
        buddy_path = fs::path(path);

        // Open the app database
        fs::path app_path = buddy_path;
        app_path.append(APP_DB);
        open_db_or_error(options, app_path.string(), &app_db);

        // Open the user database
        fs::path user_path = buddy_path;
        user_path.append(USER_DB);
        open_db_or_error(options, user_path.string(), &user_db);

    } catch (const fs::filesystem_error& e) {
        error = e.what();
        error += (std::string)do_disconnect_buddy();
    } catch (const std::runtime_error& e) {
        error = e.what();
        error += (std::string)do_disconnect_buddy();
    }

    return error;
}

String Controller::do_disconnect_buddy() {
    std::string error = "";
    if (!is_buddy_connected()) {
        error += "Buddy path is not connected";
    }
    buddy_path = fs::path();
    try {
        app_db->Close();
    }
    catch (const std::runtime_error& e) {
        error += e.what();
    }

    try {
        user_db->Close();
    }
    catch (const std::runtime_error& e) {
        error += e.what();
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












