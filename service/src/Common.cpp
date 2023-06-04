#include "Common.hpp"

std::string vectorToJson(const std::vector<std::string>& strings) {
    nlohmann::json jsonStrings = nlohmann::json(strings);
    return jsonStrings.dump();
}

std::string build_client_key(const std::string& name) {
    std::string key = R"({"category":"client","name":")" + name + "\"}";
    return key;
}

std::string build_client_value(const std::string& name, const std::string& password, const std::string& categories) {
    std::string value = R"({"name":")" + name + R"(","password":")" + password + R"(","categories":)" + categories + "}";
    return value;
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

std::string generate_json_schema(const nlohmann::json& jsonObject) {
    nlohmann::json jsonSchema = nlohmann::json::object();
    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        const std::string& key = it.key();
        const nlohmann::json& value = it.value();
        
        if (value.is_structured()) {
            jsonSchema[key] = generate_json_schema(value);
        } else {
            jsonSchema[key] = value.type_name();
        }
    }

    return jsonSchema.dump();
}
