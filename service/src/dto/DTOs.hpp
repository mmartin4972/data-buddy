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
  DTO_FIELD(String, name);
  DTO_FIELD(String, auth_token);
  DTO_FIELD(String, category_name);
  DTO_FIELD(String, key); // JSON object parsed according to key schema associated with given category
};
class GetRespDto : public oatpp::DTO {
  DTO_INIT(GetRespDto, DTO);
  DTO_FIELD(String, value); // JSON object which will be parsed by client
  DTO_FIELD(String, error);
};

class GetRangeRecvDto : public oatpp::DTO {
  DTO_INIT(GetRangeRecvDto, DTO);
  DTO_FIELD(String, name);
  DTO_FIELD(String, auth_token);
  DTO_FIELD(String, category_name);
  DTO_FIELD(String, key); // JSON object parsed according to key schema associated with given category
};
class GetRangeRespDto : public oatpp::DTO {
  DTO_INIT(GetRangeRespDto, DTO);
  DTO_FIELD(String, keys); // list of keys corresponding to queried values
  DTO_FIELD(String, values); // JSON object which will be parsed by client
  DTO_FIELD(String, error);
};

class PutRecvDto : public oatpp::DTO {
  DTO_INIT(PutRecvDto, DTO);
  DTO_FIELD(String, name);
  DTO_FIELD(String, auth_token);
  DTO_FIELD(String, category_name);
  DTO_FIELD(String, key); // JSON object parsed according to key_schema associated with category
  DTO_FIELD(String, value); // JSON object parsed according to value_schema associated with category
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

class DisconnectClientRecvDto : public oatpp::DTO {
    DTO_INIT(DisconnectClientRecvDto, DTO);
    DTO_FIELD(String, name);
    DTO_FIELD(String, auth_token);
};
class DisconnectClientRespDto : public oatpp::DTO {
    DTO_INIT(DisconnectClientRespDto, DTO);
    DTO_FIELD(String, error);
};

class AddClientRecvDto : public oatpp::DTO {
    DTO_INIT(AddClientRecvDto, DTO);
    DTO_FIELD(String, name);
    DTO_FIELD(String, auth_token);
    DTO_FIELD(String, category_name);
    DTO_FIELD(String, add_name);
};
class AddClientRespDto : public oatpp::DTO {
    DTO_INIT(AddClientRespDto, DTO);
    DTO_FIELD(String, error);
};

class CreateCategoryRecvDto : public oatpp::DTO {
    DTO_INIT(CreateCategoryRecvDto, DTO);
    DTO_FIELD(String, name);
    DTO_FIELD(String, auth_token);
    DTO_FIELD(String, category_name);
    DTO_FIELD(String, key_schema);
    DTO_FIELD(String, value_schema);
};
class CreateCategoryRespDto : public oatpp::DTO {
    DTO_INIT(CreateCategoryRespDto, DTO);
    DTO_FIELD(String, error);
};

class ListClientsRespDto : public oatpp::DTO {
    DTO_INIT(ListClientsRespDto, DTO);
    DTO_FIELD(String, error);
    DTO_FIELD(String, clients);
    // DTO_FIELD(String, client_schema); Don't think this is necessary since return all responses as json values
};

class ListCategoriesRespDto : public oatpp::DTO {
    DTO_INIT(ListCategoriesRespDto, DTO);
    DTO_FIELD(String, error);
    DTO_FIELD(String, categories);
    // DTO_FIELD(String, category_schema); Don't think necessary since return all as json values
};

#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */
