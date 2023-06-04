#include "controller/Controller.hpp"
#include <iostream>
#include <string>
#include "rocksdb/db.h"
#include "rocksdb/options.h"
#include <filesystem>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>

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

bool is_successful(String error) {
    return is_successful((std::string)error);
}

bool is_successful(std::string error) {
    return error == "";
}

Response Controller::create_response(const String& error, const oatpp::Void &dto) {
    Response res;
    if (is_successful(error)) {
        res = createDtoResponse(Status::CODE_200, dto);
    } else {
        res = createDtoResponse(Status::CODE_500, dto);
    }
    // NIce
    std::cout <<"Response cbompletely created and returning" << std::endl;
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
    // Check if the key length is valid for AES-256 (32 bytes)
    if (key.length() != 32) {
        throw std::invalid_argument("Invalid key length. Key must be 32 bytes for AES-256.");
    }

    // Generate a random IV (Initialization Vector)
    unsigned char iv[AES_BLOCK_SIZE];
    if (RAND_bytes(iv, sizeof(iv)) != 1) {
        throw std::runtime_error("Failed to generate random IV.");
    }

    // Create and initialize the cipher context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (ctx == nullptr) {
        throw std::runtime_error("Failed to create cipher context.");
    }

    // Initialize encryption operation with AES-256, CBC mode, and the provided key and IV
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                           reinterpret_cast<const unsigned char*>(key.c_str()), iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize encryption operation.");
    }

    // Calculate the required ciphertext buffer size
    int ciphertextLen = plaintext.length() + AES_BLOCK_SIZE;
    unsigned char* ciphertext = new unsigned char[ciphertextLen];

    // Encrypt the plaintext
    int len;
    if (EVP_EncryptUpdate(ctx, ciphertext, &len,
                          reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.length()) != 1) {
        delete[] ciphertext;
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to encrypt plaintext.");
    }

    int ciphertextTotalLen = len;

    // Finalize the encryption process
    if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1) {
        delete[] ciphertext;
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize encryption process.");
    }

    ciphertextTotalLen += len;

    // Append the IV at the beginning of the ciphertext
    std::string encryptedText(reinterpret_cast<const char*>(iv), AES_BLOCK_SIZE);
    encryptedText += std::string(reinterpret_cast<const char*>(ciphertext), ciphertextTotalLen);

    // Clean up
    delete[] ciphertext;
    EVP_CIPHER_CTX_free(ctx);

    return encryptedText;
}

std::string get_secret_key() {
    return "my_super_ultra_top_secret_key";
}

bool does_key_exist(RocksWrapper_ptr db, const std::string& key_schema, const std::string& key) {
    std::string tmpa, tmpb;
    std::string error = db->get(key_schema, key, "", tmpa, tmpb);
    return is_successful(error);
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

// Returns true if the client is authorized, else false
// TODO: may want to perform some sort of caching so not doing 2 gets for every request which needs authorization
bool Controller::is_client_authorized(const String& name, const String& auth_token, const String& category)  {
    std::string auth_key = build_auth_token_key(name);
    std::string provided_auth_token = build_auth_token_value(name, auth_token);
    std::string tmp, val;
    std::string error = app_db->get(AUTH_TOKEN_KEY_SCHEMA, auth_key, "", tmp, val);

    // If category is not empty perform the category check to make sure the client has access to this category
    bool client_in_category = false;
    if (category != "") { 
        try {
            std::vector<std::string> vals = get_category_values(category);
            json client_array = json::parse(vals[2]); // TODO: don't do random indexing here please. Use enums.
            for (auto& client : client_array) {
                if (client.get<std::string>() == (std::string)name) {
                    client_in_category = true;
                    break;
                }
            }
        }
        catch (const std::exception& e) {
            error = e.what();
        }
    } else {
        client_in_category = true;
    }

    return is_successful(error) && json_at<std::string>(json::parse(val)[0], AUTH_TOKEN) == provided_auth_token && client_in_category;
}

// Returns true if client is registered, else false
bool Controller::does_client_exist(const String& name) {
    std::string client_key = build_client_key(name);
    return does_key_exist(app_db, CLIENT_KEY_SCHEMA, client_key);
}

// throws std::runtime_error if error is not successful
void check_successful(const String& error) {
    if (!is_successful(error)) {
        throw std::runtime_error(error);
    }
}

std::vector<std::string> Controller::get_category_values(const String& category) {
    std::string category_key = build_category_key(category);
    std::string tmp, val;
    check_successful(app_db->get(CATEGORY_KEY_SCHEMA, category_key, "", tmp, val));
    json obj = json::parse(val);
    return {json_at<std::string>(obj, KEY_SCHEMA), json_at<std::string>(obj, VALUE_SCHEMA), json_at<std::string>(obj, CLIENTS)};
}

// overwrites existing category and corresponding client with new information. Will create new category if it doesn't exist
// REQUIRES that the client is authorized to make this request
// REQUIRSE: that the modified client exists
// TODO: for safety may want to make all database puts transactional 
// TODO: THIS FUNCTION IS WRONG!!!
String Controller::update_category(const String& client_name, const String& category, const String& key_schema, const String& value_schema, const String& clients) {
    std::string error = "";
    try {
        // add the category to the client
        std::string client_key = build_client_key(client_name);
        std::string tmp, val;
        check_successful(app_db->get(CLIENT_KEY_SCHEMA, client_key, "", tmp, val)); // Check that client exists
        json obj = json::parse(val);
        json categories = json_at<json>(obj.at(0), CATEGORIES);
        for (const auto& cat : categories) {
            if (cat.get<std::string>() == (std::string)category) {
                throw std::runtime_error("Client is already in category");
            }
        }
        categories.push_back(category);
        std::string client_value = build_client_value(client_name, json_at<std::string>(obj.at(0), PASSWORD), categories.dump());
        check_successful(app_db->put(CLIENT_KEY_SCHEMA, client_key, CLIENT_VALUE_SCHEMA, client_value));
    
        // add the client to the category
        json clients_tmp = json::parse((std::string)clients);
        clients_tmp.push_back(client_name);
        std::string category_key = build_category_key(category);
        std::string category_val = build_category_value(category, key_schema, value_schema, clients_tmp);
        check_successful(app_db->put(CATEGORY_KEY_SCHEMA, category_key, CATEGORY_VALUE_SCHEMA, category_val));
    }
    catch (const std::runtime_error& e) {
        error = e.what();
    }
    return error;
}


////////////////////////
//
//  Endpoint Functions
//
////////////////////////

String Controller::do_get(const String& name, const String& auth_token, const String& category, const String& key, const String& prefix_key, String& keys, String& values) {
    std::string error = "";
    try {
        if (!is_client_authorized(name, auth_token, category)) {
            throw std::runtime_error("Client is not authorized");
        }
        std::vector<std::string> category_vals = get_category_values(category);
        std::string keys_tmp, vals_tmp;
        user_db->get(category_vals[0], key, prefix_key, keys_tmp, vals_tmp);
        keys = keys_tmp;
        values = vals_tmp;
    }
    catch (const std::exception& e) {
        error = e.what();
    }
    return error;
}

String Controller::do_put(const String& name, String auth_token, const String& category, const String& key, const String& value) {
    std::string error = "";
    try {
        if (!is_client_authorized(name, auth_token, category)) {
            throw std::runtime_error("Client is not authorized");
        }
        std::vector<std::string> category_vals = get_category_values(category);
        user_db->put(category_vals[0], key, category_vals[1], value);
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
    std::cout << "Entering the try statement" << std::endl;
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
        user_db = RocksWrapper::create_db(user_path.string(), USER_DB);       


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
        app_db.reset();; // TODO: Should probably check that there is only one instance of this object
    }
    catch (const std::runtime_error& e) {
        error += e.what();
    }

    try {
        user_db.reset(); // TODO: should probably check that there is only one instance of this object
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
        std::string key = build_client_key(name);
        if (does_key_exist(app_db, CLIENT_KEY_SCHEMA, key)) { // TODO: you need to figure out what you get back when something doeesn't exist
            throw std::runtime_error("Client already exists");
        }
        std::string tmp;
        std::string val = build_client_value(name, password, tmp);
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
        std::string client_key = build_client_key(name);
        std::string tmp, val;
        check_successful(app_db->get(CLIENT_KEY_SCHEMA, client_key, "", tmp, val));
        json obj = json::parse(val); // This is safe since we checked successful
        // TODO: this is a security vulnerability since it means we are storing the password in plaintext
        if (json_at<std::string>(obj[0], PASSWORD) != (std::string)password) { // Check that password is correct
            throw std::runtime_error("Incorrect password");
        }

        // Generate the authentication token and store it in the app database
        std::string secret_key = get_secret_key();
        auth_token = encrypt_string(secret_key, name);
        check_successful(app_db->put(AUTH_TOKEN_KEY_SCHEMA, auth_key, AUTH_TOKEN_VALUE_SCHEMA, build_auth_token_value(name, auth_token)));
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
        if (!is_client_authorized(client_name, auth_token, category)) { // Check the client is authorized to make this request
            throw std::runtime_error("Client is not authorized");
        }
        if (does_key_exist(app_db, CATEGORY_KEY_SCHEMA, build_category_key(category))) { // check that the category exists
            throw std::runtime_error("Category does not exist");
        }
        
        // Collect existing category information
        std::vector<std::string> category_vals = get_category_values(category);

        // Perform the update
        check_successful(update_category(add_name, category, category_vals[0], category_vals[1], category_vals[2]));
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
        if (!is_client_authorized(client_name, auth_token, "")) { // Check the client is authorized to make this request
            throw std::runtime_error("Client is not authorized");
        }
        if (does_key_exist(app_db, CATEGORY_KEY_SCHEMA, build_category_key(category_name))) { // Check category doesn't already exist
            throw std::runtime_error("Category already exists");
        }
        // Create the category
        check_successful(update_category(client_name, category_name, key_schema, value_schema, "[]"));
    }
    catch (const std::runtime_error& e) {
        error = e.what();
    }
    return error;
}

// TODO: these are very similar maybe merge them?
String Controller::do_list_clients(String& clients) {
    std::string error = "";
    try {
        std::string tmp, values;
        json names = json::array();
        std::string error = app_db->get(CLIENT_KEY_SCHEMA, build_client_key(""), "name", tmp, values);
        json vals = json::parse(values);
        if (!is_successful(error) && vals.size() > 0) { // Don't thrown an error if empty
            throw std::runtime_error("Error getting clients");
        }
        for (auto& val : vals)  {
            names.push_back(json_at<std::string>(val.get<json>(), NAME));
        }
        clients = names.dump();
    }
    catch (const std::runtime_error& e) {
        error = e.what();
    }
    return error;
}

String Controller::do_list_categories(String& categories) {
    std::string error = "";
    try {
        std::string tmp, values;
        json names = json::array();
        std::string error = app_db->get(CATEGORY_KEY_SCHEMA, build_category_key(""), "name", tmp, values);
        json vals = json::parse(values);
        if (!is_successful(error) && vals.size() > 0) { // Don't thrown an error if empty
            throw std::runtime_error("Error getting clients");
        }
        for (auto& val : vals)  {
            names.push_back(json_at<std::string>(val.get<json>(), NAME));
        }
        categories = names.dump();
    }
    catch (const std::runtime_error& e) {
        error = e.what();
    }
    return error;
}













