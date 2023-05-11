#include <iostream>
#include <Wt/WApplication.h>
#include <Wt/Http/Response.h>
#include <Wt/Json/Object.h>
#include <Wt/Json/Parser.h>

using namespace Wt;

class MyApplication : public WApplication {
public:
  MyApplication(const WEnvironment& env) : WApplication(env) {
    // Connect the "json-rpc" event to the handleJsonRpc() method
    root()->setInternalPathChangeEvent("json-rpc");
    root()->internalPathChanged().connect(this, &MyApplication::handleJsonRpc);
  }

  void handleJsonRpc() {
    // Get the JSON-RPC request from the internal path
    std::string json = internalPathNextPart();

    // Parse the JSON request
    Json::Value root;
    Json::Reader reader;
    bool success = reader.parse(json, root);
    if (!success) {
      // Respond with a JSON-RPC error if the request is invalid
      respond(Json::Value(Json::objectValue)["error"] = "Invalid JSON");
      return;
    }

    // Check if the request is a JSON-RPC "ping" method
    std::string method = root.get("method", "").asString();
    if (method != "ping") {
      // Respond with a JSON-RPC error if the method is not "ping"
      respond(Json::Value(Json::objectValue)["error"] = "Unknown method");
      return;
    }

    // Respond with a JSON-RPC "hello" message
    respond(Json::Value(Json::objectValue)["result"] = "hello client");
  }

  void respond(const Json::Value& response) {
    // Convert the JSON response to a string
    std::string json = response.toStyledString();

    // Create an HTTP response with the JSON content type and body
    Http::Response response;
    response.addHeader("Content-Type", "application/json");
    response.out() << json;

    // Send the HTTP response to the client
    respond(response);
  }
};

int main(int argc, char **argv) {
  try {
    // Create a Wt application with a MyApplication instance
    WRun(argc, argv, [](const WEnvironment& env) {
      return new MyApplication(env);
    });
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
