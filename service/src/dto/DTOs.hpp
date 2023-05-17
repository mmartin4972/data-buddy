#ifndef DTOs_hpp
#define DTOs_hpp

#include "Common.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

using namespace oatpp;

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
class MyDto : public oatpp::DTO {
  
  DTO_INIT(MyDto, DTO);
  
  DTO_FIELD(String, message);
  
};

class GetTestDto : public oatpp::DTO {
  
  DTO_INIT(GetTestDto, DTO);
  
  DTO_FIELD(String, key);
  
};

class GetRecvDto : public oatpp::DTO {
  
  DTO_INIT(GetRecvDto, DTO);
  
  DTO_FIELD(String, auth_token);
  DTO_FIELD(String, group);
  DTO_FIELD(String, category);
  DTO_FIELD(String, key_params); // JSON object parsed according to category
};

class GetRespDto : public oatpp::DTO {
  
  DTO_INIT(GetRespDto, DTO);

//
  DTO_FIELD(StringVector, values); // JSON object which will be parsed by client
};

#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */
