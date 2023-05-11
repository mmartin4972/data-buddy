#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

void handle_request(tcp::socket& socket) {
    boost::system::error_code error;

    // Read the request line and headers
    std::string request;
    read_until(socket, dynamic_buffer(request), "\r\n\r\n", error);

    // Create the HTTP response
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 6\r\n"
        "\r\n"
        "Hello\n";

    // Write the response to the socket
    write(socket, buffer(response), error);
}

int main() {
    io_service service;
    tcp::endpoint endpoint(tcp::v4(), 8787);
    tcp::acceptor acceptor(service, endpoint);

    while (true) {
        tcp::socket socket(service);
        acceptor.accept(socket);
        handle_request(socket);
    }

    return 0;
}