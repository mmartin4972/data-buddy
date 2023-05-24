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

bool Controller::is_dictionary_valid(Dictionary dict, String category) {
    return true;
}

bool Controller::is_successful(String error) {
    return is_successful((std::string)error);
}

bool Controller::is_successful(std::string error) {
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

String db_put(rocksdb::DB* db, String key, String value) {
    std::string error = "";
    rocksdb::Status status = db->Put(rocksdb::WriteOptions(), key, value);
    if (!status.ok()) {
        error = status.ToString();
    }
    return error;
}

String client_put(String name, String password) {

}

////////////////////////
//
//  Endpoint Functions
//
////////////////////////

// TODO: Security vulnerability in that anyone can read the app data, which should be private
String Controller::do_create_buddy(const String& path, String& folder_path) {
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
        app_db = RocksWrapper::create_db(app_path.string(), APP_DB);       

        // Create the user database
        fs::path user_path = p;
        user_db = RocksWrapper::create_db(user_db.string(), USER_DB);       


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
        delete app_db; // TODO: Should probably check that there is only one instance of this object
    }
    catch (const std::runtime_error& e) {
        error += e.what();
    }

    try {
        delete user_db; // TODO: should probably check that there is only one instance of this object
    }
    catch (const std::runtime_error& e) {
        error += e.what();
    }
    return error;
}

String Controller::do_create_client(String name, String password, String& auth_token) {
    String error = "";
    std::string key = "client:" + (std::string)name;
    if (!is_successful(app_db->get(key, Dictionary tmp))) {
        Dictionary val = {{PASSWORD, password}};
        error += app_db->put(key, val);
        if (is_successful(error)) {
            // Read from environment a secret key. Then use user id to generate the authentication token with some encryption method
        }
    } else {
        error += "Client already exists";
    }
    return error;
}

String Controller::do_connect_client(String name, String password, String& auth_token) {
    return "";
}

String Controller::do_add_client(String auth_token, String group, String name) {
    return "";
}

String Controller::do_disconnect_client(String auth_token) {
    return "";
}

String Controller::do_create_category(String name, StringVector key_params, StringVector value_params) {
    // Check category doesn't already exist

    // Add able to be accessed by creating client
    return "";
}

String Controller::do_list_clients(StringVecVecVec& clients) {
    return "";
}

String Controller::do_list_categories(StringVecVecVec& categories) {
    return "";
}












