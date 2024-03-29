#include <iostream>

// #include "Common.hpp"
#include "dto/DTOs.hpp"
#include "controller/RocksWrapper.hpp"

#include "rocksdb/db.h"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#ifndef Controller_hpp
#define Controller_hpp

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Core logic for handling endpoint requests. Calls into the functions housed in controller.cpp
 */
class Controller : public oatpp::web::server::api::ApiController {
private:

    fs::path buddy_path; // path to the data buddy folder
    RocksWrapper_ptr app_db; // database to store app data
    RocksWrapper_ptr user_db; // database to store user data
    unsigned long long largest_client_num = 0; // largest client number used so far 

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
     * @param error: error message to be sent to client
     * @param dto: dto to be sent to client
     * RETURNS: response to be sent to client
     * REQUIRES: dto pointer must contain the error field
    */
    Response create_response(const string& error, const oatpp::Void &dto);

    /**
     * RETURNS: true if data buddy is connected, false otherwise
    */
    bool is_buddy_connected();

    /**
     * @param name: name of the client
     * @param auth_token: authentication token for the client
     * RETURNS: true if the client is authorized, false otherwise
    */
    bool is_client_authorized(const string& name, const string& auth_token);

    /**
     * @param name: name of the client
     * @param auth_token: authentication token for the client
     * @param category: name of the category to check authorization for
     * RETURNS: true if the client is authorized and they are authorized for the category, false otherwise
    */
    bool is_client_authorized_for_category(const string& name, const string& auth_token, const string& category);

    /**
     * @param category: name of the category to retreive values from
     * RETURNS: vector containing a string representing each key in the category value
    */
    std::vector<json> get_category_values(const string& category);

    // overwrites existing category and corresponding client with new information. Will create new category if it doesn't exist
    // REQUIRES that the client is authorized to make this request
    // REQUIRSE: that the modified client exists
    // TODO: for safety may want to make all database puts transactional 
    string update_category(const string& client_name, const string& category, const json& key_schema, const json& value_schema, const json& clients);


    ////////////////////////
    //
    //  Endpoint Functions
    //
    ////////////////////////

    /**
     * @param name: name of the client
     * @param auth_token: authentication token for the client
     * @param category: category associated with the key
     * @param key: key used to access the dictionary
     * @param value: value populated with a json object of all of the value retrieved by the get
     * EFFECTS: populates the value string with the values associated with the key according to the
     *  value_schema associated with the provided category
     * RETURNS: empty string if successful, error message if not
     * REQUIRES: key complies with category's key_schema 
    */
    string do_get(const string& name, const string& auth_token, const string& category, const string& key, string& value);

    /**
     * @param name: name of the client
     * @param auth_token: authentication token for the client
     * @param category: category associated with the key
     * @param key: key used to access the dictionary
     * @param keys: key populated with a json array of all of the keys retrieved by the get
     * @param values: value populated with a json array of all of the values retrieved by the get
     * EFFECTS: populates the value string with the values associated with the key according to the
     *  value_schema associated with the provided category
     * RETURNS: empty string if successful, error message if not
     * REQUIRES: key complies with category's key_schema 
    */
   // TODO: Modify this to include range based gets and such
    string do_get_range(const string& name, const string& auth_token, const string& category, const string& key, string& keys, string& values);

    /**
     * @param name: name of the client
     * @param auth_token: authentication token for the client
     * @param category: category the key is placed in to
     * @param key: json string that is queried by
     * @param value: json string that is inserted into the database
     * EFFECTS: inserts the value into the associated key
     * RETURNS: empty string if successful, error message if not
     * REQUIRES: key complies with category's key_schema 
     * REQUIRES: value complies with category's value_schema
    */
    string do_put(const string& name, string auth_token, const string& category, const string& key, const string& value);

    /**
     * @param path: absolute path to the location where the data buddy folder should be created
     * @param folder_path: absolute path to the folder that was created
     * EFFECTS: creates the data buddy folder and stores the path to the folder in folder_path.
     *   Will create a user database to store user data, and an app database to store client info, category info, and permission info
     * RETURNS: empty string if successful, error message if not
     * REQUIRES: data buddy is not currently connected
    */
    string do_create_buddy(const string& path, string& folder_path);

    /**
     * @param path: absolute path to the location of the data buddy folder
     * EFFECTS: connects to an existing data buddy folder. Will disconnect you from any currently connected data buddy folder 
     *  regardless if new connection is successful or not
     * RETURNS: empty string if successful, error message if not
     * REQUIRES: data buddy is not currently connected
    */
    string do_connect_buddy(const string& path);

    /**
     * EFFECTS: disconnects from the data buddy folder
     * RETURNS: empty string if successful, error message if not
     * REQUIRES: data buddy is currently connected
    */
    string do_disconnect_buddy();

    /**
     * @param name: name of the client
     * @param password: password of the client
     * EFFECTS: creates a new client in client database. Will populate auth_token if successful.
     * RETURNS: empty string if successful, error message if not
    */
    string do_create_client(const string& name, const string& password, string& auth_token);

    /**
     * @param name: name of the client
     * @param auth_token: authentication token for the client
     * EFFECTS: deletes the client from the client database
     * RETURNS: empty string if successful, error message if not
    */
    string do_disconnect_client(const string& name, const string& auth_token);

    /**
     * @param name: name of the client
     * @param password: password of the client
     * EFFECTS: attempts to validate client by checking client database for the provided credentials. Will populate auth_token if successful
     * RETURNS: empty string if successful, error message if not
    */
    string do_connect_client(const string& name, const string& password, string& auth_token);

    /**
     * @param client_name: name of the client requesting the add
     * @param auth_token: authentication token for the client requesting the add
     * @param category: category to add the client with 'name' to
     * @param add_name: name of the client to add to the category
     * REQUIRES: client that owns auth_token also owns category
     * EFFECTS: allows the specified client to access the specified category
     * RETURNS: empty string if successful, error message if not
    */
    string do_add_client(const string& client_name, const string& auth_token, const string& category, const string& add_name);

    /**
     * @param client_name: name of the client requesting the creation
     * @param auth_token: authentication token for the client requesting the creation
     * @param name: name of category to create
     * @param key_schema: json schema needed to construct the key for the category
     * @param value_schema: json schema needed to construct the value for the category
     * REQUIRES: category does not already exist
     * EFFECTS: creates a new category in category column family. Creator will automatically be given access to the category
     * RETURNS: empty string if successful, error message if not
    */
    string do_create_category(const string& name, const string& auth_token, const string& category_name, const string& key_schema, const string& value_schema);

    /**
     * @param clients: list of clients and their associated groups
     * @param client_schema: json schema needed to construct the client
     * RETURNS: empty string if successful, error message if not
    */
    string do_list_clients(string& clients);
    
    /**
     * @param categories: list of categories containing their name, key_params, and value_params
     * RETURNS: empty string if successful, error message if not
    */
    string do_list_categories(string& categories);

    /**
     * @param name: name of the client
     * @param auth_token: authentication token for the client
     * @param is_authenticated: boolean that is set to true if client is authenticated, false if not
     * RETURNS: empty string if successfully checked if client is authenticated, error message if not
    */
   string do_check_authenticated(const string& name, const string& auth_token, bool& is_authenticated);

    /**
     * @param is_connected: boolean that is set to true if buddy is connected, false if not
     */
    string do_check_buddy_connected(bool& is_connected);
    
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
            BODY_DTO(Object<GetRangeRecvDto>, recv)) {
        // Formatting Checks
        // TODO: complete these
        OATPP_ASSERT_HTTP(recv->auth_token, Status::CODE_400, "'auth_token' is require!");
        
        if (DEBUG) {
            printf("/db-get hit\n");
        }

        // Function Call
        string value;
        string error = do_get(recv->name, recv->auth_token, recv->category_name, recv->key, value);
        
        // Respond
        auto dto = GetRespDto::createShared();
        dto->value = value;
        dto->error = error;
        return create_response(error, dto);
    }

    ENDPOINT_INFO(get_range) {
        info->summary = "Get value for the provided key";
        info->addConsumes<Object<GetRangeRecvDto>>("application/json");
        info->addResponse<Object<GetRangeRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-get-range", get_range,
            BODY_DTO(Object<GetRangeRecvDto>, recv)) {
        // Formatting Checks
        // TODO: complete these
        OATPP_ASSERT_HTTP(recv->auth_token, Status::CODE_400, "'auth_token' is require!");
        
        if (DEBUG) {
            printf("/db-get-range hit\n");
        }

        // Function Call
        string keys;
        string values;
        string error = do_get_range(recv->name, recv->auth_token, recv->category_name, recv->key, keys, values);
        
        // Respond
        auto dto = GetRangeRespDto::createShared();
        dto->keys = keys;
        dto->values = values;
        dto->error = error;
        return create_response(error, dto);
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
        
        if (DEBUG) {
            printf("/db-put hit\n");
        }

        // Function Call
        string error = do_put(recv->name, recv->auth_token, recv->category_name, recv->key, recv->value);
        
        // Respond
        auto dto = PutRespDto::createShared();
        dto->error = error;
        return create_response(error, dto);
    }

    ENDPOINT_INFO(create_buddy) {
        info->summary = "Create a new data buddy folder";
        info->addConsumes<Object<CreateBuddyRecvDto>>("application/json");
        info->addResponse<Object<CreateBuddyRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-create-buddy", create_buddy,
            BODY_DTO(Object<CreateBuddyRecvDto>, recv)) {
        // Formatting Checks
        OATPP_ASSERT_HTTP(recv->path, Status::CODE_400, "'path' is required!");
        
        if (DEBUG) {
            printf("/db-create-buddy hit\n");
        }

        // Function Call
        string folder_path;
        string error = do_create_buddy(recv->path, folder_path);
        
        // Respond
        auto dto = CreateBuddyRespDto::createShared();
        dto->error = error;
        dto->folder_path = folder_path;
        return create_response(error, dto);
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
        
        if (DEBUG) {
            printf("/db-connect-buddy hit\n");
        }

        // Function Call
        string error = do_connect_buddy(recv->path);
        
        // Respond
        auto dto = ConnectBuddyRespDto::createShared();
        dto->error = error;
        return create_response(error, dto);
    }

    ENDPOINT_INFO(disconnect_buddy) {
        info->summary = "Disconnect from the data buddy folder";
        info->addResponse<Object<DisconnectBuddyRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/db-disconnect-buddy", disconnect_buddy) {
        
        if (DEBUG) {
            printf("/db-disconnect-buddy hit\n");
        }

        // Function Call
        string error = do_disconnect_buddy();
        
        // Respond
        auto dto = DisconnectBuddyRespDto::createShared();
        dto->error = error;
        return create_response(error, dto);
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
        
        if (DEBUG) {
            printf("/db-create-client hit\n");
        }

        // Function Call
        string auth_token;
        string error = do_create_client(recv->name, recv->password, auth_token);
        
        // Respond
        auto dto = CreateClientRespDto::createShared();
        dto->error = error;
        dto->auth_token = auth_token;
        return create_response(error, dto);
    }

    ENDPOINT_INFO(disconnect_client) {
        info->summary = "Disconnect a connected client";
        info->addConsumes<Object<DisconnectClientRecvDto>>("application/json");
        info->addResponse<Object<DisconnectClientRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-disconnect-client", disconnect_client,
            BODY_DTO(Object<DisconnectClientRecvDto>, recv)) {
        // Formatting Checks
        OATPP_ASSERT_HTTP(recv->auth_token, Status::CODE_400, "'auth_token' is require!");
        OATPP_ASSERT_HTTP(recv->name, Status::CODE_400, "'name' is require!");
        
        if (DEBUG) {
            printf("/db-disconnect-client hit\n");
        }

        // Function Call
        string error = do_disconnect_client(recv->name, recv->auth_token);
        
        // Respond
        auto dto = DisconnectClientRespDto::createShared();
        dto->error = error;
        return create_response(error, dto);
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
        
        if (DEBUG) {
            printf("/db-connect-client hit\n");
        }

        // Function Call
        string auth_token;
        string error = do_connect_client(recv->name, recv->password, auth_token);
        
        // Respond
        auto dto = ConnectClientRespDto::createShared();
        dto->error = error;
        dto->auth_token = auth_token;
        return create_response(error, dto);
    }

    ENDPOINT_INFO(add_client) {
        info->summary = "Add a client to a category";
        info->addConsumes<Object<AddClientRecvDto>>("application/json");
        info->addResponse<Object<AddClientRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-add-client", add_client,
            BODY_DTO(Object<AddClientRecvDto>, recv)) {
        // Formatting Checks
        OATPP_ASSERT_HTTP(recv->auth_token, Status::CODE_400, "'auth_token' is require!");
        OATPP_ASSERT_HTTP(recv->name, Status::CODE_400, "'name' is require!");
        OATPP_ASSERT_HTTP(recv->category_name, Status::CODE_400, "'category_name' is require!");
        OATPP_ASSERT_HTTP(recv->add_name, Status::CODE_400, "'add_name' is require!");
        
        if (DEBUG) {
            printf("/db-add-client hit\n");
        }

        // Function Call
        string error = do_add_client(recv->name, recv->auth_token, recv->category_name, recv->add_name);
        
        // Respond
        auto dto = AddClientRespDto::createShared();
        dto->error = error;
        return create_response(error, dto);
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
        OATPP_ASSERT_HTTP(recv->key_schema, Status::CODE_400, "'key_params' is require!");
        OATPP_ASSERT_HTTP(recv->value_schema, Status::CODE_400, "'value_params' is require!");
        
        if (DEBUG) {
            printf("/db-create-category hit\n");
        }

        // Function Call
        string error = do_create_category(recv->name, recv->auth_token, recv->category_name, recv->key_schema, recv->value_schema);
        
        // Respond
        auto dto = CreateCategoryRespDto::createShared();
        dto->error = error;
        return create_response(error, dto);
    }

    ENDPOINT_INFO(list_clients) {
        info->summary = "List all clients";
        info->addResponse<Object<ListClientsRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/db-list-clients", list_clients) {

        if (DEBUG) {
            printf("/db-list-clients hit\n");
        }

        // Function Call
        string clients;
        string error = do_list_clients(clients);
        
        // Respond
        auto dto = ListClientsRespDto::createShared();
        dto->error = error;
        dto->clients = clients;
        return create_response(error, dto);
    }

    ENDPOINT_INFO(list_categories) {
        info->summary = "List all categories";
        info->addResponse<Object<ListCategoriesRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/db-list-categories", list_categories) {
        
        if (DEBUG) {
            printf("/db-list-categories hit\n");
        }

        // Function Call
        string categories;
        string error = do_list_categories(categories);
        
        // Respond
        auto dto = ListCategoriesRespDto::createShared();
        dto->error = error;
        dto->categories = oatpp::String(categories.c_str(), categories.length());
        return create_response(error, dto);
    }

    ENDPOINT_INFO(ping) {
        info->summary = "Ping the server";
        info->addResponse<Object<PingRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/db-ping", ping) {
        auto dto = PingRespDto::createShared();
        dto->error = "";
        return create_response("", dto);
    }

    ENDPOINT_INFO(check_authenticated) {
        info->summary = "Ping the server and check if authenticated";
        info->addConsumes<Object<CheckAuthenticatedRecvDto>>("application/json");
        info->addResponse<Object<CheckAuthenticatedRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/db-check-authenticated", check_authenticated,
            BODY_DTO(Object<CheckAuthenticatedRecvDto>, recv)) {

        bool is_authenticated = false;
        string error = do_check_authenticated(recv->name, recv->auth_token, is_authenticated);

        auto dto = CheckAuthenticatedRespDto::createShared();
        dto->is_authenticated = is_authenticated;
        dto->error = error;
        return create_response(error, dto);
    }

    ENDPOINT_INFO(check_buddy_connected) {
        info->summary = "Check if a buddy is connected";
        info->addResponse<Object<CheckBuddyConnectedRespDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/db-check-buddy-connected", check_buddy_connected) {

        bool is_connected = false;
        string error = do_check_buddy_connected(is_connected);

        auto dto = CheckBuddyConnectedRespDto::createShared();
        dto->is_connected = is_connected;
        dto->error = error;
        return create_response(error, dto);
    }

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* Controller_hpp */
