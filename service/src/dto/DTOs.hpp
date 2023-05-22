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
  DTO_FIELD(StringVector, values); // JSON object which will be parsed by client
  DTO_FIELD(String, error);
};

class PutRecvDto : public oatpp::DTO {
  DTO_INIT(PutRecvDto, DTO);
  DTO_FIELD(String, auth_token);
  DTO_FIELD(String, group);
  DTO_FIELD(String, category);
  DTO_FIELD(String, key_params); // JSON object parsed according to category
  DTO_FIELD(String, value_params); // JSON object parsed according to category
};
class PutRespDto : public oatpp::DTO {
  DTO_INIT(PutRespDto, DTO);
  DTO_FIELD(String, error);
};

class CreateBuddyRecvDto : public oatpp::DTO {
    DTO_INIT(CreateBuddyRecvDto, DTO);
    DTO_FIELD(String, path);
};
class CreateBuddyRespDto : public oatpp::DTO {
    DTO_INIT(CreateBuddyRespDto, DTO);
    DTO_FIELD(String, error);
    DTO_FIELD(String, folder_path);
};

class ConnectBuddyRecvDto : public oatpp::DTO {
    DTO_INIT(ConnectBuddyRecvDto, DTO);
    DTO_FIELD(String, path);
};
class ConnectBuddyRespDto : public oatpp::DTO {
    DTO_INIT(ConnectBuddyRespDto, DTO);
    DTO_FIELD(String, error);
};

class DisconnectBuddyRespDto : public oatpp::DTO {
    DTO_INIT(DisconnectBuddyRespDto, DTO);
    DTO_FIELD(String, error);
};

class CreateClientRecvDto : public oatpp::DTO {
    DTO_INIT(CreateClientRecvDto, DTO);
    DTO_FIELD(String, name);
    DTO_FIELD(String, password);
};
class CreateClientRespDto : public oatpp::DTO {
    DTO_INIT(CreateClientRespDto, DTO);
    DTO_FIELD(String, error);
    DTO_FIELD(String, auth_token);
};

class ConnectClientRecvDto : public oatpp::DTO {
    DTO_INIT(ConnectClientRecvDto, DTO);
    DTO_FIELD(String, name);
    DTO_FIELD(String, password);
};
class ConnectClientRespDto : public oatpp::DTO {
    DTO_INIT(ConnectClientRespDto, DTO);
    DTO_FIELD(String, error);
    DTO_FIELD(String, auth_token);
};

class AddClientRecvDto : public oatpp::DTO {
    DTO_INIT(AddClientRecvDto, DTO);
    DTO_FIELD(String, auth_token);
    DTO_FIELD(String, group);
    DTO_FIELD(String, name);
};
class AddClientRespDto : public oatpp::DTO {
    DTO_INIT(AddClientRespDto, DTO);
    DTO_FIELD(String, error);
};

class CreateGroupRecvDto : public oatpp::DTO {
    DTO_INIT(CreateGroupRecvDto, DTO);
    DTO_FIELD(String, auth_token);
    DTO_FIELD(String, category);
    DTO_FIELD(String, group_name);
};

class CreateCategoryRecvDto : public oatpp::DTO {
    DTO_INIT(CreateCategoryRecvDto, DTO);
    DTO_FIELD(String, name);
    DTO_FIELD(StringVector, key_params);
    DTO_FIELD(StringVector, value_params);
};
class CreateCategoryRespDto : public oatpp::DTO {
    DTO_INIT(CreateCategoryRespDto, DTO);
    DTO_FIELD(String, error);
};

class ListClientsRespDto : public oatpp::DTO {
    DTO_INIT(ListClientsRespDto, DTO);
    DTO_FIELD(String, error);
    DTO_FIELD(StringVector, clients);
};

class ListCategoriesRespDto : public oatpp::DTO {
    DTO_INIT(ListCategoriesRespDto, DTO);
    DTO_FIELD(String, error);
    DTO_FIELD(StringVecVecVec, categories);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */
