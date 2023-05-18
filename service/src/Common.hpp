#include <unordered_map>
#include <string>

#include "oatpp/core/Types.hpp"

using Dictionary = std::unordered_map<std::string, std::string>;
using String = oatpp::data::mapping::type::String;
using StringVector = oatpp::data::mapping::type::Vector<String>;
using Response = std::shared_ptr<oatpp::web::server::api::ApiController::OutgoingResponse>;

const String DATA_BUDDY_FOLDER = "data_buddy";
const String APP_DB = "app_db";
const String USER_DB = "user_bd";