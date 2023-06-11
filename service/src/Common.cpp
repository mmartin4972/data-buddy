#include "Common.hpp"

string vectorToJson(const std::vector<string>& strings) {
    nlohmann::json jsonStrings = nlohmann::json(strings);
    return jsonStrings.dump();
}

json build_client_key(const string& name) {
    json key = json::parse(R"({"category":"client","name":")" + name + "\"}");
    return key;
}

json build_client_value(const string& name, const string& password, const string& categories) {
    json value = json::parse(R"({"name":")" + name + R"(","password":")" + password + R"(","categories":)" + categories + "}");
    return value;
}

json build_client_value(const string& name, const string& password) {
    json value = json::parse(R"({"name":")" + name + R"(","password":")" + password + R"(","categories":[]})");
    return value;
}

json build_category_key(const string& name) {
    json key = json::parse(R"({"category":"category","name":")" + name + "\"}");
    return key;
}

json build_category_value(const string& name, const string& key_schema, const string& value_schema, const string& clients) {
    return json::parse(R"({"name":")" + name + R"(","key_schema":)" + key_schema + R"(,"value_schema":)" + value_schema + R"(,"clients":)" + clients + "}");
}

json build_auth_token_key(const string& name) {
    json key = json::parse(R"({"category":"auth_token","name":")" + name + "\"}");
    return key;
}

json build_auth_token_value(const string& name, const string& auth_token) {
    json value = json::parse(R"({"name":")" + name + R"(","auth_token":")" + auth_token + "\"}");
    return value;
}

string generate_json_schema(const nlohmann::json& jsonObject) {
    json jsonSchema = nlohmann::json::object();
    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        const string& key = it.key();
        const json& value = it.value();
        
        if (value.is_structured()) {
            jsonSchema[key] = generate_json_schema(value);
        } else {
            jsonSchema[key] = value.type_name();
        }
    }

    return jsonSchema.dump();
}
