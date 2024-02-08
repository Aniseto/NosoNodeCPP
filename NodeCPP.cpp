// NodeCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include "P2PServer.h"
#include "SeedFunctions.h"

using namespace boost::asio;
using ip::tcp;



int main() {
    
    // STEP 1: Get Seed IP Addresses, and save them to a SeedIPAddress Vector and a local file SeedIPAddresses.txt

    std::vector<std::string> SeedIpAddresses = GetSeedIPAddresses();

    
    
    SaveToTextFile(SeedIpAddresses, "SeedIPAddresses.txt");
    
    std::cout << "Seed IP : ";
    for (const auto& ip : SeedIpAddresses) {
        std::cout << ip << std::endl;
    }



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
