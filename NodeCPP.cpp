// NodeCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <boost/asio.hpp>
#include <thread>

using namespace boost::asio;
using ip::tcp;

class P2PServer {
public:
    P2PServer(io_service& ioService, unsigned short port)
        : acceptor_(ioService, tcp::endpoint(tcp::v4(), port)),
        socket_(ioService) {
        startAccept();
    }

private:
    void startAccept() {
        acceptor_.async_accept(socket_,
            [this](const boost::system::error_code& ec) {
                if (!ec) {
                    std::cout << "Client Connected\n";

                    // Create a Thread to manage the new connection
                    std::thread([this]() {
                        handleConnection();
                        }).detach();
                }

                startAccept();
            });
    }

    void handleConnection() {
        // Manage connection from client

        // Example: Send a message to client
        std::string message = "Hello, client!";
        boost::asio::write(socket_, boost::asio::buffer(message));

        // Close the connection
        socket_.close();
        std::cout << "Connexion closed\n";
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};

int main() {
    try {
        boost::asio::io_service ioService;
        unsigned short port = 12345;  // Default Port, can be changed using argument
        P2PServer server(ioService, port);

        std::cout << "Server listening on port " << port << std::endl;

        ioService.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
