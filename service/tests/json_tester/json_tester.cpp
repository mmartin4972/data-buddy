#include <iostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>

using string = std::string;
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

json build_client_value(const string& name, const string& password, const string& categories) {
    std::string tmp = R"({"name":")" + name + R"(","password":")" + password + R"(","categories":)" + categories + "}";
    std::cout << tmp << "\n";
    json value = json::parse(tmp);
    return value;
}

bool does_json_conform_schema(const json& schema, const json& data) {
        bool result = true;
        json_schema_validator validator;
        validator.set_root_schema(schema);
        try {
            validator.validate(data);
        } catch (const std::exception& e) {
            std::cerr << "Validation failed, here is why: " << e.what() << "\n";
            result = false;
        }
        
        return result;
    }

json build_client_value(const string& name, const string& password) {
    json value = json::parse(R"({"name":")" + name + R"(","password":")" + password + R"(","categories":[]})");
    return value;
}

json build_auth_token_value(const string& name, const string& auth_token) {
    json value = json::parse(R"({"name":")" + name + R"(","auth_token":")" + auth_token + "\"}");
    return value;
}

int main() {
    string name = "test";
    string password = "test_pass";
    string categories = "[\"test_category\"]";
    json check;
    check.push_back("test_cat");
    json val_test = build_auth_token_value(name, password);
    std::cout << val_test.dump() << std::endl;
    // std::cout << does_json_conform_schema(CLIENT_VALUE_SCHEMA, val_test) << std::endl;
    return 0;
}