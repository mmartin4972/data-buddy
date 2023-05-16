#ifndef DTOs_hpp
#define DTOs_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
class MyDto : public oatpp::DTO {
  
  DTO_INIT(MyDto, DTO)
  
  DTO_FIELD(String, message);
  
};

class GetDto : public oatpp::DTO {
  
  DTO_INIT(GetDto, DTO)
  
  DTO_FIELD(String, key);
  
};

#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */
