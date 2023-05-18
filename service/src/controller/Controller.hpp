#ifndef Controller_hpp
#define Controller_hpp

#include <iostream>
#include "Common.hpp"

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
     * @param objectMapper: default object mapper used to serialize/deserialize DTOs.
     */
    Controller(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
    {}



    ////////////////////////
    //
    //  Helper Functions
    //
    ////////////////////////
    
    /**
     * EFFECTS: creates a test database and writes a key value pair to it
     * RETURNS: 0
    */
    int db_test();
    
    /**
     * REQUIRES: string is formatted as json object
     * RETURNS: dictionary with key value pairs corresponding to the json object
    */
    Dictionary string_to_dictionary(String str);
    
    /**
     * RETURNS: string formatted as json object
    */
    String dictionary_to_string(Dictionary dict);

    /**
     * RETURNS: true if the dictionary is valid for the given category, false if not 
    */
    bool is_dictionary_valid(Dictionary dict, String category);



    ////////////////////////
    //
    //  Endpoint Functions
    //
    ////////////////////////

    /**
     * @param auth_token: authentication token for the client
     * @param group: group the key is associated with
     * @param category: category the key is associated with
     * @param key_params: params used to construct the key
     * @param value_params: dictionary formatted as string with values corresponding to 
     *   names of the various category values associated with the key
     * EFFECTS: populates the value_params dictionary with the values associated with the key
     * RETURNS: empty string if successful, error message if not
    */
   // TODO: Modify this to include range based gets and such
    String do_get(String auth_token, String group, String category, Dictionary key_params, StringVector& value_params);

    /**
     * @param auth_token: authentication token for the client
     * @param group: group the key is associated with
     * @param category: category the key is associated with
     * @param key_params: array of parameters needed to construct the key for the given category
     * @param value_params: array of parameters needed to construct the value for the given category
     * REQUIRES: formatting of key corresponds with formatting of category
     * REQUIRES: formatting of value corresponds with formatting of category
     * EFFECTS: inserts the value into the associated key
     * RETURNS: empty string if successful, error message if not
    */
    String do_put(String auth_token, String group, String category, Dictionary key_params, Dictionary value_params);

    /**
     * @param path: absolute path to the location where the data buddy folder should be created
     * @param folder_path: absolute path to the folder that was created
     * EFFECTS: creates the data buddy folder and stores the path to the folder in folder_path
     * RETURNS: empty string if successful, error message if not
    */
    String do_create_buddy(String path, String& folder_path);

    /**
     * @param path: absolute path to the location of the data buddy folder
     * EFFECTS: connects to an existing data buddy folder
     * RETURNS: empty string if successful, error message if not
    */
    String do_connect_buddy(String path);

    /**
     * @param name: name of the client
     * @param password: password of the client
     * EFFECTS: creates a new client in client database. Will populate auth_token if successful.
     * RETURNS: empty string if successful, error message if not
    */
    String do_create_client(String name, String password, String& auth_token);

    /**
     * @param name: name of the client
     * @param password: password of the client
     * EFFECTS: attempts to validate client by checking client database for the provided credentials. Will populate auth_token if successful
     * RETURNS: empty string if successful, error message if not
    */
    String do_connect_client(String name, String password, String& auth_token);

    /**
     * @param auth_token: authentication token for the client requesting the add
     * @param group: group to add the client with 'name' to
     * @param name: name of the client to add to the group
     * REQUIRES: client that owns auth_token also owns group
     * EFFECTS: allows the specified client to access the specified group
     * RETURNS: empty string if successful, error message if not
    */
    String do_add_client(String auth_token, String group, String name);

    /**
     * @param auth_token: authentication token for the client
     * @param category: category associated with the group
     * @param group_name: name of group to create
     * EFFECTS: creates a new group
     * RETURNS: empty string if successful, error message if not
    */
    String do_create_group(String auth_token, String category, String group_name);   

    /**
     * @param name: name of category to create
     * @param key_params: parameters needed to construct the key for the category
     * @param value_params: parameters needed to construct the value for the category
     * EFFECTS: creates a new category in category column family
     * RETURNS: empty string if successful, error message if not
    */
    String do_create_category(String name, StringVector key_params, StringVector value_params);

    /**
     * RETURNS: list of clients
    */
    String do_list_clients();

    /**
     * RETURNS: list of groups and clients authorized to access them
    */
    String do_list_groups();
    
    /**
     * RETURNS: list of categories and their key_params and value_params
    */
    String do_list_categories();

    ////////////////////////
    //
    // Oat++ Endpoint Code
    //
    ////////////////////////

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

    ENDPOINT_INFO(get) {
        info->summary = "Get value for the provided key";
        info->addConsumes<Object<GetRecvDto>>("application/json");
        info->addResponse<Object<GetRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-get", get,
            BODY_DTO(Object<GetRecvDto>, recv)) {
        // Formatting Checks
        OATPP_ASSERT_HTTP(recv->auth_token, Status::CODE_400, "'auth_token' is require!");
        
        // Function Call
        StringVector values;
        String error = do_get(recv->auth_token, recv->group, recv->category, string_to_dictionary(recv->key_params), values);
        
        // Respond
        auto dto = GetRespDto::createShared();
        dto->values = values;
        dto->error = error;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(put) {
        info->summary = "Put provided value in the provided key. Overwrites existing value if any.";
        info->addConsumes<Object<PutRecvDto>>("application/json");
        info->addResponse<Object<PutRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-put", put,
            BODY_DTO(Object<PutRecvDto>, recv)) {
        // Formatting Checks
        OATPP_ASSERT_HTTP(recv->auth_token, Status::CODE_400, "'auth_token' is require!");
        
        // Function Call
        String error = do_put(recv->auth_token, recv->group, recv->category, string_to_dictionary(recv->key_params), string_to_dictionary(recv->value_params));
        
        // Respond
        auto dto = PutRespDto::createShared();
        dto->error = error;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(create_buddy) {
        info->summary = "Create a new data buddy folder";
        info->addConsumes<Object<CreateBuddyRecvDto>>("application/json");
        info->addResponse<Object<CreateBuddyRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-create-buddy", create_buddy,
            BODY_DTO(Object<CreateBuddyRecvDto>, recv)) {
        // Formatting Checks
        OATPP_ASSERT_HTTP(recv->path, Status::CODE_400, "'path' is require!");
        
        // Function Call
        String folder_path;
        String error = do_create_buddy(recv->path, folder_path);
        
        // Respond
        auto dto = CreateBuddyRespDto::createShared();
        dto->error = error;
        dto->folder_path = folder_path;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(connect_buddy) {
        info->summary = "Connect to an existing data buddy folder";
        info->addConsumes<Object<ConnectBuddyRecvDto>>("application/json");
        info->addResponse<Object<ConnectBuddyRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-connect-buddy", connect_buddy,
            BODY_DTO(Object<ConnectBuddyRecvDto>, recv)) {
        // Formatting Checks
        OATPP_ASSERT_HTTP(recv->path, Status::CODE_400, "'path' is require!");
        
        // Function Call
        String error = do_connect_buddy(recv->path);
        
        // Respond
        auto dto = ConnectBuddyRespDto::createShared();
        dto->error = error;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(create_client) {
        info->summary = "Create a new client";
        info->addConsumes<Object<CreateClientRecvDto>>("application/json");
        info->addResponse<Object<CreateClientRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-create-client", create_client,
            BODY_DTO(Object<CreateClientRecvDto>, recv)) {
        // Formatting Checks
        OATPP_ASSERT_HTTP(recv->name, Status::CODE_400, "'name' is require!");
        OATPP_ASSERT_HTTP(recv->password, Status::CODE_400, "'password' is require!");
        
        // Function Call
        String auth_token;
        String error = do_create_client(recv->name, recv->password, auth_token);
        
        // Respond
        auto dto = CreateClientRespDto::createShared();
        dto->error = error;
        dto->auth_token = auth_token;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(connect_client) {
        info->summary = "Connect to an existing client";
        info->addConsumes<Object<ConnectClientRecvDto>>("application/json");
        info->addResponse<Object<ConnectClientRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-connect-client", connect_client,
            BODY_DTO(Object<ConnectClientRecvDto>, recv)) {
        // Formatting Checks
        OATPP_ASSERT_HTTP(recv->name, Status::CODE_400, "'name' is require!");
        OATPP_ASSERT_HTTP(recv->password, Status::CODE_400, "'password' is require!");
        
        // Function Call
        String auth_token;
        String error = do_connect_client(recv->name, recv->password, auth_token);
        
        // Respond
        auto dto = ConnectClientRespDto::createShared();
        dto->error = error;
        dto->auth_token = auth_token;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(add_client) {
        info->summary = "Add a client to a group";
        info->addConsumes<Object<AddClientRecvDto>>("application/json");
        info->addResponse<Object<AddClientRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-add-client", add_client,
            BODY_DTO(Object<AddClientRecvDto>, recv)) {
        // Formatting Checks
        OATPP_ASSERT_HTTP(recv->auth_token, Status::CODE_400, "'auth_token' is require!");
        OATPP_ASSERT_HTTP(recv->group, Status::CODE_400, "'group' is require!");
        OATPP_ASSERT_HTTP(recv->name, Status::CODE_400, "'name' is require!");
        
        // Function Call
        String error = do_add_client(recv->auth_token, recv->group, recv->name);
        
        // Respond
        auto dto = AddClientRespDto::createShared();
        dto->error = error;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(create_group) {
        info->summary = "Create a new group";
        info->addConsumes<Object<CreateGroupRecvDto>>("application/json");
        info->addResponse<Object<CreateGroupRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-create-group", create_group,
            BODY_DTO(Object<CreateGroupRecvDto>, recv)) {
        // Formatting Checks
        OATPP_ASSERT_HTTP(recv->auth_token, Status::CODE_400, "'auth_token' is require!");
        OATPP_ASSERT_HTTP(recv->category, Status::CODE_400, "'category' is require!");
        OATPP_ASSERT_HTTP(recv->group_name, Status::CODE_400, "'group_name' is require!");
        
        // Function Call
        String error = do_create_group(recv->auth_token, recv->category, recv->group_name);
        
        // Respond
        auto dto = CreateGroupRespDto::createShared();
        dto->error = error;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(create_category) {
        info->summary = "Create a new category";
        info->addConsumes<Object<CreateCategoryRecvDto>>("application/json");
        info->addResponse<Object<CreateCategoryRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-create-category", create_category,
            BODY_DTO(Object<CreateCategoryRecvDto>, recv)) {
        // Formatting Checks
        OATPP_ASSERT_HTTP(recv->name, Status::CODE_400, "'name' is require!");
        OATPP_ASSERT_HTTP(recv->key_params, Status::CODE_400, "'key_params' is require!");
        OATPP_ASSERT_HTTP(recv->value_params, Status::CODE_400, "'value_params' is require!");
        
        // Function Call
        String error = do_create_category(recv->name, recv->key_params, recv->value_params);
        
        // Respond
        auto dto = CreateCategoryRespDto::createShared();
        dto->error = error;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(list_clients) {
        info->summary = "List all clients";
        info->addResponse<Object<ListClientsRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/db-list-clients", list_clients) {
        // Function Call
        String error = do_list_clients();
        
        // Respond
        auto dto = ListClientsRespDto::createShared();
        dto->error = error;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(list_groups) {
        info->summary = "List all groups";
        info->addResponse<Object<ListGroupsRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/db-list-groups", list_groups) {
        // Function Call
        String error = do_list_groups();
        
        // Respond
        auto dto = ListGroupsRespDto::createShared();
        dto->error = error;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(list_categories) {
        info->summary = "List all categories";
        info->addResponse<Object<ListCategoriesRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/db-list-categories", list_categories) {
        // Function Call
        String error = do_list_categories();
        
        // Respond
        auto dto = ListCategoriesRespDto::createShared();
        dto->error = error;
        return createDtoResponse(Status::CODE_200, dto);
    }
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* Controller_hpp */
