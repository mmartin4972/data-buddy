#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>

using json = nlohmann::json;
using json_schema_validator = nlohmann::json_schema::json_validator;

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

json build_client_key(const std::string& name) {
    json key = json::parse(R"({"category":"client","name":")" + name + "\"}");
    return key;
}

int main() {
    std::string name = "test";
    json key_test = build_client_key(name);
    std::cout << key_test.dump() << std::endl;
    return 0;
}