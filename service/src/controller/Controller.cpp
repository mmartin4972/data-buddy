#include "controller/Controller.hpp"
#include <iostream>
#include <string>
#include "rocksdb/db.h"
#include "rocksdb/options.h"
#include <filesystem>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

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
    string key = "hello";
    string value = "world";
    status = db->Put(rocksdb::WriteOptions(), key, value);
    if (!status.ok()) {
        std::cerr << "Error writing to database: " << status.ToString() << std::endl;
        return 1;
    }

    // Read the value back
    string result;
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

bool is_successful(string error) {
    return error == "";
}

Response Controller::create_response(const string& error, const oatpp::Void &dto) {
    Response res;
    if (is_successful(error)) {
        res = createDtoResponse(Status::CODE_200, dto);
    } else {
        res = createDtoResponse(Status::CODE_500, dto);
    }
    return res;
}

void open_db_or_error(const rocksdb::Options &options, const string &name, rocksdb::DB **dbptr) {
    rocksdb::Status status = rocksdb::DB::Open(options, name, dbptr);
    if (!status.ok()) {
        throw std::runtime_error(status.ToString());
    }
}

bool Controller::is_buddy_connected() {
    return buddy_path.empty() == false;
}

std::string base64Encode(const unsigned char* data, int length) {
    BIO* bmem = BIO_new(BIO_s_mem());
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO_push(b64, bmem);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(b64, data, length);
    BIO_flush(b64);

    BUF_MEM* bptr;
    BIO_get_mem_ptr(b64, &bptr);

    std::string result(bptr->data, bptr->length);
    BIO_free_all(b64);

    return result;
}

string encrypt_string_base64(const string& key, const string& plaintext) {
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
    string encryptedText(reinterpret_cast<const char*>(iv), AES_BLOCK_SIZE);
    encryptedText += string(reinterpret_cast<const char*>(ciphertext), ciphertextTotalLen);

    // Clean up
    delete[] ciphertext;
    EVP_CIPHER_CTX_free(ctx);

    // Encode the ciphertext in base64
    string encryptedTextBase64 = base64Encode(reinterpret_cast<const unsigned char*>(encryptedText.c_str()),
                                              encryptedText.length());
    return encryptedTextBase64;
}

string get_secret_key() {
    // NOTE: should be 32 bytes for AES-256
    // Ex: abcdefghijklmnopqrstuvwxyz123456
    // string secret_key = std::getenv("DATA_BUDDY_KEY");
    // std::cout << "Got secret key: " << secret_key << "\n";
    return "01234567890123456789012345678901";
;
}

bool does_key_exist(RocksWrapper_ptr db, const json& key_schema, const json& key) {
    json tmpa, tmpb;
    string error = db->get(key_schema, key, tmpa, tmpb);
    return is_successful(error);
}

// Returns true if the client is authorized, else false
// TODO: may want to perform some sort of caching so not doing 2 gets for every request which needs authorization
bool Controller::is_client_authorized(const string& name, const string& auth_token)  {
    json provided_auth_token = build_auth_token_value(name, auth_token);
    json tmp, val;
    string error = app_db->get(AUTH_TOKEN_KEY_SCHEMA, build_auth_token_key(name), tmp, val);
    return is_successful(error) && val[0] == provided_auth_token;
}

bool Controller::is_client_authorized_for_category(const string& name, const string& auth_token, const string& category) {
    string error = "";
    bool is_client_in_category = false;
    try {
        // Check that the client is authorized
        if (!is_client_authorized(name, auth_token)) {
            throw std::runtime_error("Client is not authorized");
        }

        //Check that category is not empty
        if (category =="") {
            throw std::runtime_error("Category is empty");
        }

        // Perform the category check to make sure the client has access to this category
        std::vector<string> vals = get_category_values(category);
        json client_array = json::parse(vals[2]); // TODO: don't do random indexing here please. Use enums.
        for (auto& client : client_array) {
            if (client.get<string>() == (string)name) {
                is_client_in_category = true;
                break;
            }
        }
    }
    catch (const std::exception& e) {
        error = e.what();
    }
    return is_successful(error) && is_client_in_category;
}

// throws std::runtime_error if error is not successful
void check_successful(const string& error) {
    if (!is_successful(error)) {
        throw std::runtime_error(error);
    }
}

std::vector<string> Controller::get_category_values(const string& category) {
    json tmp, val;
    check_successful(app_db->get(CATEGORY_KEY_SCHEMA, build_category_key(category), tmp, val));
    return {json_at<string>(val, KEY_SCHEMA), json_at<string>(val, VALUE_SCHEMA), json_at<string>(val, CLIENTS)};
}

// updates category key so that authorized includes the new client and updates the client key so that categories includes the new category
// REQUIRES that the client is authorized to make this request
// REQUIRSE: that the modified client exists
// TODO: for safety may want to make all database puts transactional 
// TODO: THIS FUNCTION IS WRONG!!!
string Controller::update_category(const string& client_name, const string& category, const string& key_schema, const string& value_schema, const string& clients) {
    string error = "";
    try {
        // add the category to the client
        json client_key = build_client_key(client_name);
        json tmp, val;
        check_successful(app_db->get(CLIENT_KEY_SCHEMA, client_key, tmp, val)); // Check that client exists
        val = json_at<json>(val, 0); // Remove from array
        json categories = json_at<json>(val, CATEGORIES);
        for (const auto& cat : categories) {
            if (cat.get<string>() == (string)category) {
                throw std::runtime_error("Client is already in category");
            }
        }
        categories.push_back(category);
        json client_value = build_client_value(client_name, json_at<string>(val, PASSWORD), categories.dump());
        check_successful(app_db->put(CLIENT_KEY_SCHEMA, client_key, CLIENT_VALUE_SCHEMA, client_value));
    
        // add the client to the category
        json clients_cur = json::parse(clients);
        clients_cur.push_back(client_name);
        json category_key = build_category_key(category);
        json category_val = build_category_value(category, key_schema, value_schema, clients_cur.dump());
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

// string Controller::do_get(const string& name, const string& auth_token, const string& category, const string& key, const string& prefix_key, string& keys, string& values) {
//     string error = "";
//     try {
//         if (!is_client_authorized_for_category(name, auth_token, category)) {
//             throw std::runtime_error("Client is not authorized");
//         }
//         std::vector<string> category_vals = get_category_values(category);
//         string keys_tmp, vals_tmp;
//         user_db->get(json::parse(category_vals[0]), json::parse(key), prefix_key, keys_tmp, vals_tmp);
//         keys = keys_tmp;
//         values = vals_tmp;
//     }
//     catch (const std::exception& e) {
//         error = e.what();
//     }
//     return error;
// }

// string Controller::do_put(const string& name, string auth_token, const string& category, const string& key, const string& value) {
//     string error = "";
//     try {
//         if (!is_client_authorized_for_category(name, auth_token, category)) {
//             throw std::runtime_error("Client is not authorized");
//         }
//         std::vector<string> category_vals = get_category_values(category);
//         user_db->put(category_vals[0], key, category_vals[1], value);
//     }
//     catch (const std::exception& e) {
//         error = e.what();
//     }
//     return error;
// }

// TODO: Security vulnerability in that anyone can read the app data, which should be private
string Controller::do_create_buddy(const string& path, string& folder_path) {
    string error = "";
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
        user_db = RocksWrapper::create_db(user_path.string(), USER_DB);       


    } catch (const fs::filesystem_error& e) {
        error = e.what();
        error += (string)do_disconnect_buddy();

    } catch (const std::runtime_error& e) {
        error = e.what();
        error += (string)do_disconnect_buddy();
    }

    return error;
}

string Controller::do_connect_buddy(const string& path) {
    string error = "";
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
        error += (string)do_disconnect_buddy();
    } catch (const std::runtime_error& e) {
        error = e.what();
        error += (string)do_disconnect_buddy();
    }

    return error;
}

string Controller::do_disconnect_buddy() {
    string error = "";
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

string Controller::do_create_client(const string& name, const string& password, string& auth_token) {
    // TODO: maybe we don't hardcode strings here
    string error = "";
    try {
        json key = build_client_key(name);
        if (does_key_exist(app_db, CLIENT_KEY_SCHEMA, key)) { // TODO: you need to figure out what you get back when something doeesn't exist
            throw std::runtime_error("Client already exists");
        }
        json val = build_client_value(name, password);
        check_successful(app_db->put(CLIENT_KEY_SCHEMA, key, CLIENT_VALUE_SCHEMA, val));
        check_successful(do_connect_client(name, password, auth_token));
    }
    catch (const std::runtime_error& e) {
        error += e.what();
    }
    return error;
}

string Controller::do_disconnect_client(const string& name, const string& auth_token) {
    string error = "";
    try {
        if (!is_client_authorized(name, auth_token)) { // Check the client is authorized to make this request
            throw std::runtime_error("Client is not authorized");
        }
        json auth_key = build_auth_token_key(name);
        check_successful(app_db->del(AUTH_TOKEN_KEY_SCHEMA, auth_key));
    }
    catch (const std::runtime_error& e) {
        error += e.what();
    }
    return error;
}

// Read the secret key from the environment variable
// Use the client's name to then generate them an authentication token
// Store the authentication token in the app database
string Controller::do_connect_client(const string& name, const string& password, string& auth_token) {
    string error = "";
    try {
        // Validation Checks
        json auth_key = build_auth_token_key(name);
        if (does_key_exist(app_db, AUTH_TOKEN_KEY_SCHEMA, auth_key)) { // Check client doesn't already have an authentication token
            throw std::runtime_error("Client already has an authentication token");
        }
        json client_key = build_client_key(name);
        json tmp, val;
        check_successful(app_db->get(CLIENT_KEY_SCHEMA, client_key, tmp, val));

        // TODO: this is a security vulnerability since it means we are storing the password in plaintext
        if (json_at<string>(val[0], PASSWORD) != (string)password) { // Check that password is correct
            throw std::runtime_error("Incorrect password");
        }

        // Generate the authentication token and store it in the app database
        string secret_key = get_secret_key();
        auth_token = encrypt_string_base64(secret_key, name);
        check_successful(app_db->put(AUTH_TOKEN_KEY_SCHEMA, auth_key, AUTH_TOKEN_VALUE_SCHEMA, build_auth_token_value(name, auth_token)));
    }
    catch (const std::runtime_error& e) {
        error += e.what();
    }
    return error;
}

// string Controller::do_add_client(const string& client_name, const string& auth_token, const string& category, const string& add_name) {
//     string error = "";
//     try {
//         // Validation Checks
//         if (!is_client_authorized_for_category(client_name, auth_token, category)) { // Check the client is authorized to make this request
//             throw std::runtime_error("Client is not authorized");
//         }
//         if (does_key_exist(app_db, CATEGORY_KEY_SCHEMA, build_category_key(category))) { // check that the category exists
//             throw std::runtime_error("Category does not exist");
//         }
        
//         // Collect existing category information
//         std::vector<string> category_vals = get_category_values(category);

//         // Perform the update
//         check_successful(update_category(add_name, category, category_vals[0], category_vals[1], category_vals[2]));
//     }
//     catch (const std::runtime_error& e) {
//         error = e.what();
//     }
//     return error;
// }

string Controller::do_create_category(const string& client_name, const string& auth_token, const string& category_name, const string& key_schema, const string& value_schema) {
    string error = "";
    try {
        if (!is_client_authorized(client_name, auth_token)) { // Check the client is authorized to make this request
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
string Controller::do_list_clients(string& clients) {
    string error = "";
    try {
        json tmp, values;
        string error = app_db->get(CLIENT_KEY_SCHEMA, build_client_key(""), tmp, values);
        if (!is_successful(error) && values.size() > 0) { // Don't thrown an error if empty
            throw std::runtime_error("Error getting clients");
        }

        json names = json::array();
        for (auto& val : values)  {
            names.push_back(json_at<string>(val.get<json>(), NAME));
        }
        clients = names.dump();
    }
    catch (const std::runtime_error& e) {
        error = e.what();
    }
    return error;
}

string Controller::do_list_categories(string& categories) {
    string error = "";
    try {
        json tmp, values;
        string error = app_db->get(CATEGORY_KEY_SCHEMA, build_category_key(""), tmp, values);
        if (!is_successful(error) && values.size() > 0) { // Don't thrown an error if empty
            throw std::runtime_error("Error getting clients");
        }
        categories = values.dump();
    }
    catch (const std::runtime_error& e) {
        error = e.what();
    }
    return error;
}
