#include <unordered_map>
#include <string>

#include <boost/filesystem.hpp> // TODO: may want to change this to filesystem for c++17 and compile with -lstdc++fs
#include <filesystem>
#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>
#include <iostream>

#include "oatpp/core/Types.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

#ifndef Common_hpp
#define Common_hpp

//////////////////////
//
// ALIAS DEFINITIONS
//
//////////////////////
using string = std::string;
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
const string DATA_BUDDY_FOLDER = "data_buddy";
const string APP_DB = "app_db";
const string USER_DB = "user_db";
const string CATEGORY = "category";
const string PASSWORD = "password";
const string AUTH_TOKEN = "auth_token";
const string KEY_SCHEMA = "key_schema";
const string VALUE_SCHEMA = "value_schema";
const string CLIENTS = "clients";
const string CATEGORIES = "categories";
const string NAME = "name";

static json CLIENT_KEY_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "category": { "type": "string", "enum": ["client"] },
                "name": { "type": "string" }
            },
            "required": ["category", "name"]
        }
    )"_json; // TODO you may want to incoporate std::format here so constants can be used

static json CLIENT_VALUE_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "name": { "type": "string" },
                "password": { "type": "string" },
                "categories": { "type": "array",
                    "items": { "type": "string" }
                }
            },
            "required": ["name", "password", "categories"]
        }
    )"_json;

static json CATEGORY_KEY_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "category": { "type": "string", "enum": ["category"] },
                "name": { "type": "string" }
            },
            "required": ["category", "name"]
        }
    )"_json;

static json CATEGORY_VALUE_SCHEMA = R"(
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
    )"_json;

static json AUTH_TOKEN_KEY_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "category": { "type": "string", "enum": ["auth_token"] },
                "name": { "type": "string" }
            },
            "required": ["category", "name"]
        }
    )"_json;

static json AUTH_TOKEN_VALUE_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "name": { "type": "string" },
                "auth_token": { "type": "string" }
            },
            "required": ["auth_token"]
        }
    )"_json;

// Key schema to be used for all keys in the user database
// The key object should be formatted as the key schema specified for the category?
static json USER_KEY_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "category": { "type": "string"},
                "key": { "type": "object" }
            },
            "required": ["category", "key"]
        }
    )"_json;

static json USER_VALUE_SCHEMA = R"(
        {
            "type": "object",
            "properties": {
                "key": { "type": "object" },
                "value": { "type": "object" }
            },
            "required": ["key","value"]
        }
    )"_json;

//////////////////////
//
// HELPER FUNCTIONS
//
/////////////////////

string vectorToJson(const std::vector<string>& strings);

json build_client_key(const string& name);

json build_client_value(const string& name, const string& password, const string& categories);

json build_client_value(const string& name, const string& password);

json build_category_key(const string& name);

json build_category_value(const string& name, const string& key_schema, const string& value_schema, const std::vector<string>& clients);

json build_auth_token_key(const string& name);

json build_auth_token_value(const string& name, const string& auth_token);

string generate_json_schema(const nlohmann::json& jsonObject);

#endif /* Common_hpp */
