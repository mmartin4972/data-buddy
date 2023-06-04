#include <unordered_map>
#include <string>

#include <boost/filesystem.hpp> // TODO: may want to change this to filesystem for c++17 and compile with -lstdc++fs
#include <filesystem>
#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>

#include "oatpp/core/Types.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

#ifndef Common_hpp
#define Common_hpp

//////////////////////
//
// ALIAS DEFINITIONS
//
//////////////////////
using String = oatpp::data::mapping::type::String;
using Response = std::shared_ptr<oatpp::web::server::api::ApiController::OutgoingResponse>;
namespace fs = std::filesystem;
class RocksWrapper;
using RocksWrapper_ptr = std::shared_ptr<RocksWrapper>;
using json_schema_validator = nlohmann::json_schema::json_validator;
using json = nlohmann::json;

//////////////////////
//
// CONSTANTS
//
/////////////////////
#define ROCKSDB_MAX_KEY_SIZE 1024
#define ROCKSDB_MAX_VALUE_SIZE 65535
const std::string DATA_BUDDY_FOLDER = "data_buddy";
const std::string APP_DB = "app_db";
const std::string USER_DB = "user_db";
const std::string CATEGORY = "category";
const std::string PASSWORD = "password";
const std::string AUTH_TOKEN = "auth_token";
const std::string KEY_SCHEMA = "key_schema";
const std::string VALUE_SCHEMA = "value_schema";
const std::string CLIENTS = "clients";
const std::string CATEGORIES = "categories";
const std::string NAME = "name";

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

// Key schema to be used for all keys in the user database
// The key object should be formatted as the key schema specified for the category?
const std::string USER_KEY_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "category": { "type": "string"},
                "key": { "type": "object" },
            },
            "required": ["category", "key"]
        }
    )";

const std::string USER_VALUE_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "key": { "type": "object" },
                "value": { "type": "object" },
            },
            "required": ["key","value"]
        }
    )";

//////////////////////
//
// HELPER FUNCTIONS
//
/////////////////////

std::string vectorToJson(const std::vector<std::string>& strings);

std::string build_client_key(const std::string& name);

std::string build_client_value(const std::string& name, const std::string& password, const std::string& categories);

std::string build_category_key(const std::string& name);

std::string build_category_value(const std::string& name, const std::string& key_schema, const std::string& value_schema, const std::vector<std::string>& clients);

std::string build_auth_token_key(const std::string& name);

std::string build_auth_token_value(const std::string& name, const std::string& auth_token);

std::string generate_json_schema(const nlohmann::json& jsonObject);

#endif /* Common_hpp */
