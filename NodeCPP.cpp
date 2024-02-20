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
    //std::string ProgramVersion="0.4.2";
    //std::string Subversion="Cb1";

    // STEP 1: Get Seed IP Addresses, and save them to a SeedIPAddress Vector and a local file SeedIPAddresses.txt

    std::vector<std::string> SeedIpAddresses = GetSeedIPAddresses();
    SaveToTextFile(SeedIpAddresses, "SeedIPAddresses.txt");
    std::cout << "Total Seed Nodes : " << SeedIpAddresses.size() << std::endl;

    //Create a vector of Nodes with structure Node to save data , IP, Port, NodeStatus, MerkleTree. .. etc

    
    std::vector<Node> SeedNodes;
    std::vector<std::thread> threads;

    for (int i = 0; i < SeedIpAddresses.size(); i++) {
        // Crear un nuevo nodo con la dirección IP y el puerto predeterminados
        Node SeedNode(SeedIpAddresses[i], DefaultSeedPort);

        // Lanzar un hilo para llamar a SetNodeStatus para el nodo actual
        threads.emplace_back([&SeedNode]() {
            SeedNode.SetNodeStatus();
            SeedNode.CalculateMerkle();
         
            //SeedNodes.push_back(SeedNode);
            //SeedNode.CalculateMerkle();
            });

       
        // Agregar el nodo al vector de nodos semilla
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        SeedNodes.push_back(SeedNode);
    }


    //Print Seed Nodes in table format
    std::cout << std::left << std::setw(15) << "Node IP" << std::setw(10) << "Node Port" << std::setw(15) << "Node Merkle" << std::endl;
    for (int i = 0; i < SeedNodes.size(); i++) {
        SeedNodes[i].PrintNodeTable();
        
    }

    std::string UTCTime = GetUTCTimeFromNTPServer();
    std::cout << "\nUTC Time: " << UTCTime << std::endl;
    
    std::string NodePublicIP = GetNodePublicIP();
    std::cout << "Public IP: " << NodePublicIP << std::endl;




    //STEP2 Get Nodestatus from all Seed nodes, and verify all fields are equal on all seed nodes.
    // GetSeedNodeConsensus. only if more more than 75% nodes are equal you can go on. Mínimum 3 seed nodes ??

 //STEP3 Get all Nodes running NOSOCFG command
    //std::string NOSOCFG_COMMAND = "NSLMNS\n";
    
    
    
    std::string NOSOCFG_COMMAND = "NOSOCFG\n";
    std::string NodeList = SendStringToNode(SeedIpAddresses[0], DefaultSeedPort, NOSOCFG_COMMAND);
    std::cout << "Node List: " << NodeList << std::endl;

    //Print NodePresentation


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
