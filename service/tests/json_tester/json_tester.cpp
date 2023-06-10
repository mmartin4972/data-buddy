#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "../../src/Common.hpp"
#include <vector>

// using string = std::string;
// using json = nlohmann::json;
// using json_schema_validator = nlohmann::json_schema::json_validator;

// static json CLIENT_KEY_SCHEMA = R"(
//         {
//             "type": "object",
//             "properties": {
//                 "category": { "type": "string", "enum": ["client"] },
//                 "name": { "type": "string" }
//             },
//             "required": ["category", "name"]
//         }
//     )"_json; // TODO you may want to incoporate std::format here so constants can be used

// static json CLIENT_VALUE_SCHEMA = R"(
//         {
//             "type": "object",
//             "properties": {
//                 "name": { "type": "string" },
//                 "password": { "type": "string" },
//                 "categories": { "type": "array",
//                     "items": { "type": "string" }
//                 }
//             },
//             "required": ["name", "password", "categories"]
//         }
//     )"_json;

// static json CATEGORY_KEY_SCHEMA = R"(
//         {
//             "type": "object",
//             "properties": {
//                 "category": { "type": "string", "enum": ["category"] },
//                 "name": { "type": "string" }
//             },
//             "required": ["category", "name"]
//         }
//     )"_json;

// static json CATEGORY_VALUE_SCHEMA = R"(
//         {
//             "type": "object",
//             "properties": {
//                 "name": { "type": "string" },
//                 "key_schema": { "type": "object" },
//                 "value_schema": { "type": "object" },
//                 "clients": { "type": "array",
//                     "items": { "type": "string" }
//                 }
//             },
//             "required": ["key_schema", "value_schema", "clients"]
//         }
//     )"_json;

// static json AUTH_TOKEN_KEY_SCHEMA = R"(
//         {
//             "type": "object",
//             "properties": {
//                 "category": { "type": "string", "enum": ["auth_token"] },
//                 "name": { "type": "string" }
//             },
//             "required": ["category", "name"]
//         }
//     )"_json;

// static json AUTH_TOKEN_VALUE_SCHEMA = R"(
//         {
//             "type": "object",
//             "properties": {
//                 "name": { "type": "string" },
//                 "auth_token": { "type": "string" }
//             },
//             "required": ["auth_token"]
//         }
//     )"_json;

// // Key schema to be used for all keys in the user database
// // The key object should be formatted as the key schema specified for the category?
// static json USER_KEY_SCHEMA = R"(
//         {
//             "type": "object",
//             "properties": {
//                 "category": { "type": "string"},
//                 "key": { "type": "object" }
//             },
//             "required": ["category", "key"]
//         }
//     )"_json;

// static json USER_VALUE_SCHEMA = R"(
//         {
//             "type": "object",
//             "properties": {
//                 "key": { "type": "object" },
//                 "value": { "type": "object" }
//             },
//             "required": ["key","value"]
//         }
//     )"_json;

// For all put requests to app_db we need to require the category value to be "category_name"
// The user is free to add any additional keys on to that.
// Category must be the FIRST value
static json TRANSACTION_KEY_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "category": { "type": "string" },
                "time": { 
                    "type": "string",
                    "format": "date-time"
                }
            },
            "required": ["category","time"]
        }
    )"_json;

// json build_client_key(const std::string& name) {
//     json key = json::parse(R"({"category":"client","name":")" + name + "\"}");
//     return key;
// }

// json build_client_value(const string& name, const string& password, const string& categories) {
//     std::string tmp = R"({"name":")" + name + R"(","password":")" + password + R"(","categories":)" + categories + "}";
//     std::cout << tmp << "\n";
//     json value = json::parse(tmp);
//     return value;
// }

json build_transaction_key(const string& category, const string& time) {
    json key = json::parse(R"({"category":")" + category + R"(","time":")" + time + "\"}");
    return key;
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

// json build_client_value(const string& name, const string& password) {
//     json value = json::parse(R"({"name":")" + name + R"(","password":")" + password + R"(","categories":[]})");
//     return value;
// }

// json build_auth_token_value(const string& name, const string& auth_token) {
//     json value = json::parse(R"({"name":")" + name + R"(","auth_token":")" + auth_token + "\"}");
//     return value;
// }

// const string CATEGORY = "category";

// string build_query_key_from_json(const json& key_schema, const json& key) {
//     string query_key = "";
//     string delimiter = "::";
//     // Base schema validation
//     if (!does_json_conform_schema(key_schema, key)) {
//         throw std::runtime_error("The given key does not conform to the given key schema");
//     }

//     // Check that the category is the first property
//     const json& requiredKeys = json_at<json>(key_schema, "required");
//     if (json_at<string>(requiredKeys, 0) != CATEGORY) {
//         throw std::runtime_error("The category property must be the first property in the key schema");
//     }

//     // Check that properties that there are no non-empty properties after an empty property
//     bool found_empty = false;
//     for (const auto& req_key : requiredKeys) {
//         string key_value = json_at<string>(key, req_key.get<string>()); // ERROR!! Make sure that you use the json get method here when putting in RocksWrapper
//         if (found_empty && key_value != "") {
//             throw std::runtime_error("The key " + req_key.dump() + " is not allowed to be non-empty when a prior key was already empty");
//         } else if (key_value == "") {
//             std::cout << "Found empty key: " << req_key.dump() << "\n";
//             found_empty = true;
//         } else {
//             std::cout << "Key_vlue: " << key_value << "\n";
//             query_key += key_value + delimiter;
//         }
//     }
//     query_key = query_key.substr(0, query_key.size() - delimiter.size());

//     // Check that the query_key size is less than the max key size
//     // if (query_key.size() > max_key_size) {
//     //     throw std::runtime_error("The constructed key is too large");
//     // }

//     return query_key;
// }

int main() {
    // auto now = std::chrono::system_clock::now();

    // // Convert the system time to a time_t object
    // std::time_t time = std::chrono::system_clock::to_time_t(now);

    // // Convert the time_t object to a string
    // std::stringstream ss;
    // ss << std::ctime(&time);
    // std::string currentTime = ss.str();
    // Get the current system time
    // Get the current system time
    auto now = std::chrono::system_clock::now();

    // Convert the system time to a time_t object
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    // Convert the time_t object to a string
    std::tm* timeInfo = std::gmtime(&time);  // Use gmtime for UTC time
    char buffer[21];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", timeInfo);
    std::string currentTime(buffer);

    const string key_schema = R"({"type": "object", "properties": {"category": {"type": "string"}, "time": {"type": "string", "format": "date-time"}}, "required": ["category", "time"]})";
    const string val_schema = R"({"type": "object", "properties": {"name": {"type": "string"}, "amount": {"type": "number"}, "time": {"type": "string", "format": "date-time"}, "place": {"type": "string"}}, "required": ["name", "amount", "time", "place"]})";
    const string clients = R"(["test client"])";
    json val = build_category_value("test_category", key_schema, val_schema, clients);

    // std::cout << "Current time: " << currentTime << "\n";
    // json transaction_key = build_transaction_key("nice", currentTime);
    // std::cout << "Transaction key: " << transaction_key.dump() << "\n";

    std::cout << does_json_conform_schema(CATEGORY_VALUE_SCHEMA, val) << "\n";

    return 0;
}

// g++ -std=c++17 json_tester.cpp ../../src/Common.cpp -lnlohmann_json_schema_validator -o test
