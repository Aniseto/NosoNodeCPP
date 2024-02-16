// NodeCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include "P2PServer.h"
#include "SeedFunctions.h"
#include "DataStructures.h"

using namespace boost::asio;
using ip::tcp;



int main() {
    

    int DefaultSeedPort = 8080;
    int DefaultNodePort = 8080;

    // STEP 1: Get Seed IP Addresses, and save them to a SeedIPAddress Vector and a local file SeedIPAddresses.txt

    std::vector<std::string> SeedIpAddresses = GetSeedIPAddresses();
    SaveToTextFile(SeedIpAddresses, "SeedIPAddresses.txt");
    std::cout << "Total Seed Nodes : " << SeedIpAddresses.size() << std::endl;



    //STEP2 Get Nodestatus from all Seed nodes, and verify all fields are equal on all seed nodes.
    // GetSeedNodeConsensus. only if more more than 75% nodes are equal you can go on. Mínimum 3 seed nodes ??

   /*//NODESTATUS 1{Peers} 2{LastBlock} 3{Pendings} 4{Delta} 5{headers} 6{version} 7{UTCTime} 8{MNsHash}
//           9{MNscount} 10{LasBlockHash} 11{BestHashDiff} 12{LastBlockTimeEnd} 13{LBMiner}
//           14{ChecksCount} 15{LastBlockPoW} 16{LastBlockDiff} 17{summary} 18{GVTs} 19{nosoCFG}
//           20{PSOHash}*/

//STEP 2: Select a random seed server from the SeedIPAddress Vector and get all Nodes IP
    try {
        boost::asio::io_service ioService;
        unsigned short port = 12345;  // Default Port, can be changed using argument
        P2PServer server(ioService, port);

        std::cout << "-Server listening on port " << port << std::endl;

        ioService.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
