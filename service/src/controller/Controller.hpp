#ifndef Controller_hpp
#define Controller_hpp

#include "dto/DTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

// using namespace oatpp::data::mapping::type; // TODO: remove this
using oat_u_map = oatpp::data::mapping::type::UnorderedMap<oatpp::data::mapping::type::String, oatpp::data::mapping::type::String>;
using oat_d_get_recv = oatpp::data::mapping::type::DTOWrapper<GetRecvDto>;
using oat_d_get_resp = oatpp::data::mapping::type::DTOWrapper<GetRespDto>;


/**
 * Core logic for handling endpoint requests. Calls into the functions housed in controller.cpp
 */
class Controller : public oatpp::web::server::api::ApiController {
public:
    /**
     * Constructor with object mapper.
     * @param objectMapper: default object mapper used to serialize/deserialize DTOs.
     */
    Controller(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
    {}

    /**
     * 
     * Helper Functions
     * 
     */
    int db_test();
    oat_u_map do_get(oat_d_get_recv recv);
    
    /**
     * 
     * End Points
     * REQUIRES: The data buddy service is running and listening for connections
    */

    ENDPOINT_INFO(root) {
        info->summary = "Test Endpoint 1";
        info->addResponse<Object<MyDto>>(Status::CODE_200, "application/json");
    }  
    ENDPOINT("GET", "/db-hello", root) {
        auto dto = MyDto::createShared();
        dto->message = "Hello World!";
        return createDtoResponse(Status::CODE_200, dto);
    }


    ENDPOINT_INFO(get_test) {
        info->summary = "Get value for the provided key";
        // info->addConsumes<Object<GetDto>>("application/json");
        info->addResponse<Object<GetTestDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-get-test", get_test,
            BODY_DTO(Object<GetTestDto>, recv)) {
        OATPP_ASSERT_HTTP(recv->key, Status::CODE_400, "'key' is require!");
        db_test();
        auto dto = GetTestDto::createShared();
        dto->key = recv->key;
        return createDtoResponse(Status::CODE_200, dto);
    }

    /**
     * @param auth_token: authentication token for the client
     * @param group: group the key is associated with
     * @param category: category the key is associated with
     * @param key: key to get the value for
     * EFFECTS: returns the value associated with the key
     * RETURNS: the value associated with the key
    */
   // TODO: Modify this to include range based gets and such
    ENDPOINT_INFO(get) {
        info->summary = "Get value for the provided key";
        info->addConsumes<Object<GetRecvDto>>("application/json");
        info->addResponse<Object<GetRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-get", get,
            BODY_DTO(Object<GetRecvDto>, recv)) {
        OATPP_ASSERT_HTTP(recv->auth_token, Status::CODE_400, "'auth_token' is require!");
        oat_u_map vals = do_get(recv);
        oat_d_get_resp dto = GetRespDto::createShared();
        // dto->value = vals;
        return createDtoResponse(Status::CODE_200, dto);
    }

    // /**
    //  * @param auth_token: authentication token for the client
    //  * @param group: group the key is associated with
    //  * @param category: category the key is associated with
    //  * @param key_params: array of parameters needed to construct the key for the given category
    //  * @param value_params: array of parameters needed to construct the value for the given category
    //  * REQUIRES: formatting of key corresponds with formatting of category
    //  * REQUIRES: formatting of value corresponds with formatting of category
    // */
    // ENDPOINT_INFO(put) {
    //     // info->summary = "Put provided value in the provided key. Overwrites existing value if any.";
    //     // info->addConsumes<Object<Dto>>("application/json");
    //     // info->addResponse<Object<PutResDto>>(Status::CODE_200, "application/json");
    // }

    // /**
    //  * @param client_name: name of the client
    //  * @param password: password of the client
    //  * EFFECTS: creates a new client and opens a socket that the client will use to communicate with the data buddy service
    //  * RETURNS: authentication token for the client if successful, empty string if not
    // */
    // ENDPOINT_INFO(create_client) {}

    // /**
    //  * @param client_name: name of the client
    //  * @param password: password of the client
    //  * EFFECTS: opens a socket that the client will use to communicate with the data buddy service
    //  * RETURNS: authentication token for the client if successful, empty string if not
    // */
    // ENDPOINT_INFO(connect_client) {}

    // /**
    //  * @param auth_token: authentication token for the client
    //  * @param category: category associated with the group
    //  * @param group_name: group to create
    //  * EFFECTS: creates a new group
    //  * RETURNS: true if successful, false if not
    // */
    // ENDPOINT_INFO(create_group) {}

    // /**
    //  * @param name: name of category to create
    //  * @param key_params: parameters needed to construct the key for the category
    //  * @param value_params: parameters needed to construct the value for the category
    // */
    // ENDPOINT_INFO(create_category) {}

    // /**
    //  * RETURNS: list of clients
    //  */
    // ENDPOINT_INFO(list_clients) {}

    // /**
    //  * RETURNS: list of groups
    // */
    // ENDPOINT_INFO(list_groups) {}

    // /**
    //  * RETURNS: list of categories and their key_params and value_params
    // */
    // ENDPOINT_INFO(list_categories) {}

    // /**
    //  * @param path: absolute path to the location where the data buddy folder should be created
    //  * EFFECTS: creates the data buddy folder
    //  * RETURNS: the absolute path to the folder
    // */
    // ENDPOINT_INFO(create_buddy) {}
    // /**
    //  * @param path: absolute path to the location of the data buddy folder
    //  * EFFECTS: connects to an existing data buddy folder freturns true if success and false if fails
    // */
    // ENDPOINT_INFO(connect_buddy) {}


  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* Controller_hpp */
