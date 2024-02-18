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

    //Create a vector of Nodes with structure Node to save data , IP, Port, NodeStatus, MerkleTree. .. etc

    std::vector<Node> SeedNodes;
    for (int i = 0; i < SeedIpAddresses.size(); i++) {
		//NodeStatusData NodeStatus;

		Node SeedNode(SeedIpAddresses[i], DefaultSeedPort);
        std::cout << "Calling SetNodeStatus" << std::endl;
        SeedNode.SetNodeStatus();
        std::cout  << "Calling CalculateMerkle" << std::endl;
        SeedNode.CalculateMerkle();
        //SeedNode.GetNodeStatus();
        std::cout << "Calling Push";
		SeedNodes.push_back(SeedNode);

	}

 

    //Print Seed Nodes in table format
    std::cout << std::left << std::setw(15) << "Node IP" << std::setw(10) << "Node Port" << std::setw(15) << "Node Merkle" << std::endl;
    for (int i = 0; i < SeedNodes.size(); i++) {
        SeedNodes[i].PrintNodeTable();
    }
    





    //STEP2 Get Nodestatus from all Seed nodes, and verify all fields are equal on all seed nodes.
    // GetSeedNodeConsensus. only if more more than 75% nodes are equal you can go on. Mínimum 3 seed nodes ??

 

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
