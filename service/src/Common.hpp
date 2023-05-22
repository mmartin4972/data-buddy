#ifndef Common_hpp
#define Common_hpp

#include <unordered_map>
#include <string>


#include "oatpp/core/Types.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

using Dictionary = std::unordered_map<std::string, std::string>;
using String = oatpp::data::mapping::type::String;
using StringVector = oatpp::data::mapping::type::Vector<String>;
using Response = std::shared_ptr<oatpp::web::server::api::ApiController::OutgoingResponse>;
using StringVecVecVec = oatpp::data::mapping::type::Vector<oatpp::data::mapping::type::Vector<StringVector>>;

const std::string DATA_BUDDY_FOLDER = "data_buddy";
const std::string APP_DB = "app_db";
const std::string USER_DB = "user_db";

#endif /* Common_hpp */