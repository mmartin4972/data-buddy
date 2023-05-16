#ifndef Controller_hpp
#define Controller_hpp

#include "dto/DTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Core logic for handling endpoint requests. Calls into the functions housed in controller.cpp
 */
class Controller : public oatpp::web::server::api::ApiController {
public:
    /**
     * Constructor with object mapper.
     * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
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
    
    /**
     * 
     * End Points
     * 
    */

    ENDPOINT_INFO(root) {
        info->summary = "Test Endpoint 1";
    }  
    ENDPOINT("GET", "/db-hello", root) {
        auto dto = MyDto::createShared();
        // dto->statusCode = 200;
        dto->message = "Hello World!";
        return createDtoResponse(Status::CODE_200, dto);
    }


    ENDPOINT_INFO(get) {
        info->summary = "Get value for the provided key";
        info->addConsumes<Object<GetDto>>("application/json");
        info->addResponse<Object<GetDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-get-test", get,
            BODY_DTO(Object<GetDto>, recv)) {
        OATPP_ASSERT_HTTP(recv->key, Status::CODE_400, "'key' is require!");
        db_test();
        auto dto = GetDto::createShared();
        // dto->statusCode = 200;
        dto->key = recv->key;
        return createDtoResponse(Status::CODE_200, dto);
    }


  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* Controller_hpp */
