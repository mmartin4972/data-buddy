#ifndef Common_hpp
#define Common_hpp

#include <unordered_map>
#include <string>
#include <filesystem>

#include "controller/Controller.hpp"

#include "oatpp/core/Types.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

// using Dictionary = std::unordered_map<std::string, std::string>;
using String = oatpp::data::mapping::type::String;
// using StringVector = oatpp::data::mapping::type::Vector<String>;
using Response = std::shared_ptr<oatpp::web::server::api::ApiController::OutgoingResponse>;
// using StringVecVecVec = oatpp::data::mapping::type::Vector<oatpp::data::mapping::type::Vector<StringVector>>;
using RocksWrapper_ptr = std::shared_ptr<RocksWrapper>;

const std::string DATA_BUDDY_FOLDER = "data_buddy";
const std::string APP_DB = "app_db";
const std::string USER_DB = "user_db";
const std::string CATEGORY = "category";
const std::string PASSWORD = "password";
const std::string AUTH_TOKEN = "auth_token";

const std::string CLIENT_KEY_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "category": { "type": "string", "enum": ["client"] },
                "name": { "type": "string" },
            },
            "required": ["category", "name"]
        }
    )"; // TODO you may want to incoporate std::format here so constants can be used

const std::string CLIENT_VALUE_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "name": { "type": "string" },
                "password": { "type": "string" },
                "categories": { "type": "array",
                    "items": { "type": "string" }
                }
            },
            "required": ["password", "categories"]
        }
    )";

const std::string CATEGORY_KEY_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "category": { "type": "string", "enum": ["category"] },
                "name": { "type": "string" },
            },
            "required": ["category", "name"]
        }
    )";

const std::string CATEGORY_VALUE_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "name": { "type": "string" },
                "key_schema": { "type": "object" },
                "value_schema": { "type": "object" },
                "clients": { "type": "array",
                    "items": { "type": "string" }
                }
            },
            "required": ["key_schema", "value_schema", "clients"]
        }
    )";

const std::string AUTH_TOKEN_KEY_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "category": { "type": "string", "enum": ["auth_token"] },
                "name": { "type": "string" },
            },
            "required": ["category", "name"]
        }
    )";

const std::string AUTH_TOKEN_VALUE_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "name": { "type": "string" },
                "auth_token": { "type": "string" },
            },
            "required": ["auth_token"]
        }
    )";

std::string vectorToJson(const std::vector<std::string>& strings) {
    json jsonStrings = json(strings);
    return jsonStrings.dump();
}

std::string build_client_key(const std::string& name) {
    std::string key = R"({"category":"client","name":")" + name + "\"}";
    return key;
}

std::string build_client_value(const std::string& name, const std::string& password, const std::vector<std::string>& categories) {
    std::string value = R"({"name":")" + name + R"(","password":")" + password + R"(","categories":)" + vectorToJson(categories) + "}";
    return value
}

std::string build_category_key(const std::string& name) {
    std::string key = R"({"category":"category","name":")" + name + "\"}";
    return key;
}

std::string build_category_value(const std::string& name, const std::string& key_schema, const std::string& value_schema, const std::vector<std::string>& clients) {
    std::string value = R"({"name":)" + name + R"(,"key_schema":)" + key_schema + R"(,"value_schema":)" + value_schema + R"(,"clients":)" + vectorToJson(clients) + "}";
    return value;
}

std::string build_auth_token_key(const std::string& name) {
    std::string key = R"({"category":"auth_token","name":")" + name + "\"}";
    return key;
}

std::string build_auth_token_value(const std::string& name, const std::string& auth_token) {
    std::string value = R"({"name":)" + name + R"(","auth_token":")" + auth_token + "\"}";
    return value;
}

std::string generate_json_schema(const json& jsonObject) {
    json jsonSchema = json::object();
    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        const std::string& key = it.key();
        const json& value = it.value();
        
        if (value.is_structured()) {
            jsonSchema[key] = generateJsonSchema(value);
        } else {
            jsonSchema[key] = value.type_name();
        }
    }

    return jsonSchema.dump();
}

using fs = std::filesystem;

namespace Common {

#define ROCKSDB_MAX_KEY_SIZE 1024
#define ROCKSDB_MAX_VALUE_SIZE 65535

// Dictionary string_to_dictionary(String string) {
//     Dictionary dict;
//     std::string jsonString = (std::string)string;
//     json jsonData = json::parse(jsonString);

//     for (json::iterator it = jsonData.begin(); it != jsonData.end(); ++it) {
//         if (it.value().is_string()) {
//             dict[it.key()] = it.value().get<std::string>();
//         }
//     }

//     return dict;
// }

// String dictionary_to_string(Dictionary dict) {
//     json jsonMap;

//     for (const auto& pair : dict) {
//         jsonMap[pair.first] = pair.second;
//     }

//     return jsonMap.dump();
// }

}

#endif /* Common_hpp */