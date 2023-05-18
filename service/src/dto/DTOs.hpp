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

class RespDto : public oatpp::DTO {
  DTO_INIT(RespDto, DTO);
  DTO_FIELD(String, error);
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
class GetRespDto : public RespDto {
  DTO_INIT(GetRespDto, DTO);
  DTO_FIELD(StringVector, values); // JSON object which will be parsed by client
};

class PutRecvDto : public oatpp::DTO {
  DTO_INIT(PutRecvDto, DTO);
  DTO_FIELD(String, auth_token);
  DTO_FIELD(String, group);
  DTO_FIELD(String, category);
  DTO_FIELD(String, key_params); // JSON object parsed according to category
  DTO_FIELD(String, value_params); // JSON object parsed according to category
};
class PutRespDto : public RespDto {
  DTO_INIT(PutRespDto, DTO);
};

class CreateBuddyRecvDto : public oatpp::DTO {
    DTO_INIT(CreateBuddyRecvDto, DTO);
    DTO_FIELD(String, path);
};
class CreateBuddyRespDto : public RespDto {
    DTO_INIT(CreateBuddyRespDto, DTO);
    DTO_FIELD(String, folder_path);
};

class ConnectBuddyRecvDto : public oatpp::DTO {
    DTO_INIT(ConnectBuddyRecvDto, DTO);
    DTO_FIELD(String, path);
};
class ConnectBuddyRespDto : public RespDto {
    DTO_INIT(ConnectBuddyRespDto, DTO);
};

class CreateClientRecvDto : public oatpp::DTO {
    DTO_INIT(CreateClientRecvDto, DTO);
    DTO_FIELD(String, name);
    DTO_FIELD(String, password);
};
class CreateClientRespDto : public RespDto {
    DTO_INIT(CreateClientRespDto, DTO);
    DTO_FIELD(String, auth_token);
};

class ConnectClientRecvDto : public oatpp::DTO {
    DTO_INIT(ConnectClientRecvDto, DTO);
    DTO_FIELD(String, name);
    DTO_FIELD(String, password);
};
class ConnectClientRespDto : public RespDto {
    DTO_INIT(ConnectClientRespDto, DTO);
    DTO_FIELD(String, auth_token);
};

class AddClientRecvDto : public oatpp::DTO {
    DTO_INIT(AddClientRecvDto, DTO);
    DTO_FIELD(String, auth_token);
    DTO_FIELD(String, group);
    DTO_FIELD(String, name);
};
class AddClientRespDto : public RespDto {
    DTO_INIT(AddClientRespDto, DTO);
};

class CreateGroupRecvDto : public oatpp::DTO {
    DTO_INIT(CreateGroupRecvDto, DTO);
    DTO_FIELD(String, auth_token);
    DTO_FIELD(String, category);
    DTO_FIELD(String, group_name);
};
class CreateGroupRespDto : public RespDto {
    DTO_INIT(CreateGroupRespDto, DTO);
};

class CreateCategoryRecvDto : public oatpp::DTO {
    DTO_INIT(CreateCategoryRecvDto, DTO);
    DTO_FIELD(String, name);
    DTO_FIELD(StringVector, key_params);
    DTO_FIELD(StringVector, value_params);
};
class CreateCategoryRespDto : public RespDto {
    DTO_INIT(CreateCategoryRespDto, DTO);
};

class ListClientsRespDto : public RespDto {
    DTO_INIT(ListClientsRespDto, DTO);
    DTO_FIELD(StringVector, clients);
};

class ListGroupsRespDto : public RespDto {
    DTO_INIT(ListGroupsRespDto, DTO);
    DTO_FIELD(oatpp::data::mapping::type::Vector<oatpp::data::mapping::type::Vector<StringVector>>, groups);
};

class ListCategoriesRespDto : public RespDto {
    DTO_INIT(ListCategoriesRespDto, DTO);
    DTO_FIELD(oatpp::data::mapping::type::Vector<oatpp::data::mapping::type::Vector<StringVector>>, categories);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */
