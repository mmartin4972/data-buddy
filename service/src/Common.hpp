#ifndef Common_hpp
#define Common_hpp

#include <unordered_map>
#include <string>
#include <filesystem>

#include "controller/Controller.hpp"

#include "oatpp/core/Types.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

using Dictionary = std::unordered_map<std::string, std::string>;
using String = oatpp::data::mapping::type::String;
using StringVector = oatpp::data::mapping::type::Vector<String>;
using Response = std::shared_ptr<oatpp::web::server::api::ApiController::OutgoingResponse>;
using StringVecVecVec = oatpp::data::mapping::type::Vector<oatpp::data::mapping::type::Vector<StringVector>>;
using RocksWrapper_ptr = std::shared_ptr<RocksWrapper>;

const std::string DATA_BUDDY_FOLDER = "data_buddy";
const std::string APP_DB = "app_db";
const std::string USER_DB = "user_db";
const std::string CATEGORY = "category";
const std::string PASSWORD = "password";

using fs = std::filesystem;

namespace Common {

Dictionary string_to_dictionary(String string) {
    Dictionary dict;
    std::string jsonString = (std::string)string;
    json jsonData = json::parse(jsonString);

    for (json::iterator it = jsonData.begin(); it != jsonData.end(); ++it) {
        if (it.value().is_string()) {
            dict[it.key()] = it.value().get<std::string>();
        }
    }

    return dict;
}

String dictionary_to_string(Dictionary dict) {
    json jsonMap;

    for (const auto& pair : dict) {
        jsonMap[pair.first] = pair.second;
    }

    return jsonMap.dump();
}

}

#endif /* Common_hpp */