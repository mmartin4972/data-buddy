#ifndef Controller_hpp
#define Controller_hpp

#include "dto/DTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include <iostream>
#include <string>
#include "rocksdb/db.h"
#include "rocksdb/options.h"

using namespace rocksdb;

int db_test() {
    DB* db;
    Options options;
    options.create_if_missing = true;

    // Open the database
    Status status = DB::Open(options, "/tmp/testdb", &db);
    if (!status.ok()) {
        std::cerr << "Error opening database: " << status.ToString() << std::endl;
        return 1;
    }

    // Write a key-value pair
    std::string key = "hello";
    std::string value = "world";
    status = db->Put(WriteOptions(), key, value);
    if (!status.ok()) {
        std::cerr << "Error writing to database: " << status.ToString() << std::endl;
        return 1;
    }

    // Read the value back
    std::string result;
    status = db->Get(ReadOptions(), key, &result);
    if (!status.ok()) {
        std::cerr << "Error reading from database: " << status.ToString() << std::endl;
        return 1;
    }
    std::cout << "Value for key " << key << ": " << result << std::endl;

    // Close the database
    delete db;

    return 0;
}

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
public:
  
  ENDPOINT("GET", "/db-hello", root) {
    auto dto = MyDto::createShared();
    dto->statusCode = 200;
    dto->message = "Hello World!";
    return createDtoResponse(Status::CODE_200, dto);
  }
  
  ENDPOINT_INFO(get) {
    info->summary = "Get value for the provided key";
    info->addConsumes<Object<GetDto>>("application/json");
    info->addResponse<Object<GetDto>>(Status::CODE_200, "application/json");
  }
  ENDPOINT("POST", "/db-get", get,
           BODY_DTO(Object<GetDto>, recv)) {
    OATPP_ASSERT_HTTP(recv->key, Status::CODE_400, "'key' is require!");
    db_test();
    return createDtoResponse(Status::CODE_200, recv->key);
  }
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* Controller_hpp */
