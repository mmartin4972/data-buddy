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

std::string encrypt_string(const std::string& key, const std::string& plaintext) {
  // Get the size of the key.
  size_t key_size = EVP_BytesToKey(EVP_aes_256_cbc(), NULL, key.c_str(), key.length(), 1, NULL, NULL);

  // Create an AES cipher object.
  EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

  // Initialize the cipher object with the key.
  if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.c_str(), NULL)) {
    std::cerr << "Error initializing cipher object." << std::endl;
    return "";
  }

  // Encrypt the plaintext.
  std::vector<unsigned char> ciphertext(plaintext.length() + EVP_CIPHER_CTX_block_size(ctx));
  size_t ciphertext_len;
  if (!EVP_EncryptUpdate(ctx, ciphertext.data(), &ciphertext_len, plaintext.c_str(), plaintext.length())) {
    std::cerr << "Error encrypting plaintext." << std::endl;
    return "";
  }

  // Finalize the encryption.
  if (!EVP_EncryptFinal_ex(ctx, ciphertext.data() + ciphertext_len, &ciphertext_len)) {
    std::cerr << "Error finalizing encryption." << std::endl;
    return "";
  }

  // Return the encrypted string.
  ciphertext.resize(ciphertext_len);
  return std::string((char*)ciphertext.data());
}

std::string get_secret_key() {

}

bool does_key_exist(RocksWrapper_ptr db, const std::string& key_schema, const std::string& key) {
    std::string error = db->get(key_schema, key, "", &std::vector<std::string>());
    return is_successful(error);
}

// Returns true if the client is authorized, else false
bool is_client_authorized(const String& name, const String& auth_token)  {
    std::auth_key = build_auth_token_key(name);
    std::provided_auth_token = build_auth_token_value(name, auth_token);
    std::vector<std::string> val;
    std::string error = app_db->get(AUTH_TOKEN_KEY_SCHEMA, auth_key, "", val);
    return is_successful(error) && val[0] == provided_auth_token;
}

// Returns true if client is registered, else false
bool does_client_exist(const String& name) {
    std::string client_key = build_client_key(name);
    return does_key_exist(app_db, CLIENT_KEY_SCHEMA, client_key);
}

// throws std::runtime_error if error is not successful
void check_successful(const String& error) {
    if (!is_successful(error)) {
        throw std::runtime_error(error);
    }
}

// Returns the object stored at the key
// Throws std::runtime_error if key does not exist
template<typename T>
T json_at(json obj, const String& key) {
    if (obj.find(key) == obj.end()) {
        throw std::runtime_error("Key does not exist");
    }
    return obj.at(key).get<T>();
}

////////////////////////
//
//  Endpoint Functions
//
////////////////////////

String Controller::do_get(const String& name, const String& auth_token, const String& key_schema, const String& key, const String& prefix_key, String& value) {
    std::string error = "";
    try {
        if (!is_client_authorized(name, auth_token)) {
            throw std::runtime_error("Client is not authorized");
        }
        std::vector<std::string> vals;
        user_db->get(key_schema, key, prefix_key, vals);
        json jsonArray(vals.begin(), vals.end());
        value = jsonArray.dump();
    }
    catch (const std::exception& e) {
        error = e.what();
    }
    return error;
}

String Controller::do_put(const String& name, String auth_token, const String& key_schema, const String& key, const String& value_schema, const String& value) {
    std::string error = "";
    try {
        if (!is_client_authorized(name, auth_token)) {
            throw std::runtime_error("Client is not authorized");
        }
        user_db->put(key_schema, key, value_schema, value);
    }
    catch (const std::exception& e) {
        error = e.what();
    }
    return error;
}

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

String Controller::do_connect_buddy(const String& path) {
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
        app_db = RocksWrapper::open_db(app_path.string());

        // Open the user database
        fs::path user_path = buddy_path;
        user_path.append(USER_DB);
        user_db = RocksWrapper::open_db(user_path.string());

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

String Controller::do_create_client(const String& name, const String& password, String& auth_token) {
    // TODO: maybe we don't hardcode strings here
    std::string error = "";
    try {
        std::string client_key = build_client_key(name);
        if (does_key_exist(app_db, CLIENT_KEY_SCHEMA, key)) { // TODO: you need to figure out what you get back when something doeesn't exist
            throw std::runtime_error("Client already exists");
        }
        std::string val = build_client_value(name, password, std::vector<string>());
        check_successful(app_db->put(CLIENT_KEY_SCHEMA, key, CLIENT_VALUE_SCHEMA, val));
        check_successful(do_connect_client(name, password, auth_token));
    }
    catch (const std::runtime_error& e) {
        error += e.what();
    }
    return error;
}

// Read the secret key from the environment variable
// Use the client's name to then generate them an authentication token
// Store the authentication token in the app database
String Controller::do_connect_client(const String& name, const String& password, String& auth_token) {
    std::string error = "";
    try {
        // Validation Checks
        std::string auth_key = build_auth_token_key(name);
        if (does_key_exist(app_db, AUTH_TOKEN_KEY_SCHEMA, auth_key)) { // Check client doesn't already have an authentication token
            throw std::runtime_error("Client already has an authentication token");
        }
        std::client_key = build_client_key(name);
        std::vector<std::string> val;
        check_successful(app_db->get(CLIENT_KEY_SCHEMA, client_key, "", val));
        json obj = json::parse(val[0]); // This is safe since we checked successful
        // TODO: this is a security vulnerability since it means we are storing the password in plaintext
        if (json_at<std::string>(val, PASSWORD) != (std::string)password) { // Check that password is correct
            throw std::runtime_error("Incorrect password");
        }

        // Generate the authentication token and store it in the app database
        std::string secret_key = get_secret_key();
        auth_token = encrypt_string(secret_key, name);
        check_successful(app_db->put(AUTH_TOKEN_KEY_SCHEMA, key, AUTH_TOKEN_VALUE_SCHEMA, build_auth_token_value(name, auth_token)));
    }
    catch (const std::runtime_error& e) {
        error += e.what();
    }
    return error;
}

String Controller::do_add_client(const String& client_name, const String& auth_token, const String& category, const String& add_name) {
    std::string error = "";
    try {
        // Validation Checks
        if (!is_client_authorized(client_name, auth_token)) { // Check the client is authorized to make this request
            throw std::runtime_error("Client is not authorized");
        }
        if (does_key_exist(app_db, CATEGORY_KEY_SCHEMA, build_category_key(category))) { // check that the category exists
            throw std::runtime_error("Category does not exist");
        }
        
        // Collect existing category information
        std::string category_key = build_category_key(category);
        std::vector<std::string> category_val;
        check_successful(app_db->get(CATEGORY_KEY_SCHEMA, category_key, "", category_val));
        json category_obj = json::parse(category_val[0]);
        std::vector<std::string> clients = json_at<std::vector<std::string>>(category_obj, CLIENTS);
        
        // Perform the update
        check_successful(update_category(add_name, category, json_at<std::string>(category_obj, KEY_SCHEMA), json_at<std::string>(category_obj, VALUE_SCHEMA), clients));
    }
    catch (const std::runtime_error& e) {
        error = e.what();
    }
    return error;
}

// overwrites existing category and corresponding client with new information. Will create new category if it doesn't exist
// REQUIRES that the client is authorized to make this request
// REQUIRSE: that the modified client exists
// TODO: for safety may want to make all database puts transactional 
String update_category(const String& client_name, const String& category, const String& key_schema, const String& value_schema, const std::vector<std::string> clients) {
    std::string error = "";
    try {
        // add the category to the client
        std::string client_key = build_client_key(client_name);
        std::vector<std::string> val;
        check_successful(app_db->get(CLIENT_KEY_SCHEMA, client_key, val)); // Check that client exists
        json obj = json::parse(val[0]);
        std::vector<std::string> categories = json_at<std::vector<std::string>>(obj, CATEGORIES);
        if (std::find(categories.begin(), categories.end(), category) != categories.end()) { // Check is already in category
            throw std::runtime_error("Client is already in category");
        }
        categories.push_back(category);
        std::string client_value = build_client_value(client_name, json_at<std::string>(obj, PASSWORD), categories);
        check_successful(app_db->put(CLIENT_KEY_SCHEMA, client_key, CLIENT_VALUE_SCHEMA, client_value));
    
        // add the client to the category
        clients.push_back(client_name);
        std::string category_key = build_category_key(category);
        std::string category_val = build_category_value(category, key_schema, value_schema, clients);
        check_successful(app_db->put(CATEGORY_KEY_SCHEMA, category_key, CATEGORY_VALUE_SCHEMA, category_val));
    }
    catch (const std::runtime_error& e) {
        error = e.what();
    }
    return error;
}

// TODO: Implement this
// String Controller::do_disconnect_client(const String& client_name, const String& auth_token) {
//     return "";
// }

String Controller::do_create_category(const String& client_name, const String& auth_token, const String& category_name, const String& key_schema, const String& value_schema) {
    std::string error = "";
    try {
        if (!is_client_authorized(client_name, auth_token)) { // Check the client is authorized to make this request
            throw std::runtime_error("Client is not authorized");
        }
        if (does_key_exist(app_db, CATEGORY_KEY_SCHEMA, build_category_key(name))) { // Check category doesn't already exist
            throw std::runtime_error("Category already exists");
        }
        // Create the category
        check_successful(update_category(client_name, category_name, key_schema, value_schema, std::vector<std::string>()));
    }
    catch (const std::runtime_error& e) {
        error = e.what();
    }
    return error;
}

// TODO: these are very similar maybe merge them?
String Controller::do_list_clients(String& clients, String& client_schema) {
    std::string error = "";
    try {
        std::vector<std::string> values;
        json names = json::array();
        std::string error = app_db->get(CLIENT_KEY_SCHEMA, build_client_key(""), "name", values);
        if (!is_successful(error) && values.size() > 0) { // Don't thrown an error if empty
            throw std::runtime_error("Error getting clients");
        }
        for (std::string& val : values)  {
            json obj = json::parse(val);
            names.push_back(json_at<std::string>(obj, NAME));
        }
        clients = names.dump();
        client_schema = generate_json_schema(names);
    }
    catch (const std::runtime_error& e) {
        error = e.what();
    }
    return error;
}

String Controller::do_list_categories(String& categories, String& category_schema) {
    std::string error = "";
    try {
        std::vector<std::string> values;
        json names = json::array();
        std::string error = app_db->get(CATEGORY_KEY_SCHEMA, build_category_key(""), "name", values);
        if (!is_successful(error) && values.size() > 0) { // Don't thrown an error if empty
            throw std::runtime_error("Error getting categories");
        }
        for (std::string& val : values)  {
            json obj = json::parse(val);
            names.push_back(json_at<std::string>(obj, NAME));
        }
        categories = names.dump();
        category_schema = generate_json_schema(names);
    }
    catch (const std::runtime_error& e) {
        error = e.what();
    }
    return error;
}












