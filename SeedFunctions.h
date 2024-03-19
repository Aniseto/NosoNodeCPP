#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <boost/uuid/detail/md5.hpp>
#include <botan/hash.h>
#include <botan/hex.h>
#include <botan/auto_rng.h>
#include <process.h>
#include <botan/md5.h>
//#include <botan/hex.h>
//#include <string>
#include <algorithm> // Para std::transform
#include <cctype>    // Para std::toupper
//#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <ctime>
#include <future>
//#include <NtpClient.h>
//#include <ntpclient/NtpClient.h>

//#include <arpa/inet.h>
//#include <netdb.h>

std::vector<std::string> ReadSeedIPAddressesFromFile(const std::string& filename) {
    std::vector<std::string> ipAddresses;
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file" << filename << std::endl;
        //If file does not exist, return a default trusted seed nodes.
        ipAddresses.push_back("20.199.50.27");
        ipAddresses.push_back("4.233.61.8");
        return ipAddresses;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        ipAddresses.push_back(line);
    }

    inputFile.close();
    return ipAddresses;
}

void SaveToTextFile(const std::vector<std::string>& ipAddresses, const std::string& filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the file " << filename << " so save data" << std::endl;
        return;
    }

    //  Save each address to the file
    for (const auto& ip : ipAddresses) {
        outputFile << ip << std::endl;
    }

    outputFile.close();
}

std::vector<std::string> GetSeedIPAddresses() {
    std::string domain = "testnet.nosocoin.com";
    std::vector<std::string> ipAddresses;

    struct addrinfo hints, * res, * p;
    int status;

    // Setting the search criteria
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET o AF_INET6 to force IPv4 o IPv6
    hints.ai_socktype = SOCK_STREAM;

    // Make DNS query
    if ((status = getaddrinfo(domain.c_str(), NULL, &hints, &res)) != 0) {
        std::cerr << "Error getting information: " << gai_strerror(status) << std::endl;
        return ipAddresses;
    }

    // Go to all results and get IP
    for (p = res; p != NULL; p = p->ai_next) {
        void* addr;
        std::string ipString;

        // Geting IPV4 address ir IPV6
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
            addr = &(ipv4->sin_addr);
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, addr, ip, sizeof(ip));
            ipString = ip;
        }
        else { // IPv6
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            char ip[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, addr, ip, sizeof(ip));
            ipString = ip;
        }

        // Add address to vector
        ipAddresses.push_back(ipString);
    }

    freeaddrinfo(res); // Free memory 
    //std::cout << "Seed Vector has #elements : " << ipAddresses.size() << std::endl;
    if (ipAddresses.size() == 0)
    {
        std::cerr << "Error getting IP addresses from " << domain << std::endl;
        ipAddresses = ReadSeedIPAddressesFromFile("SeedIPAddresses.txt");
    }
 
    return ipAddresses;
}


std::string calculateMD5(const std::string& input) {
    
   
    Botan::MD5 hash_md5;
    Botan::secure_vector<uint8_t> hashResult = hash_md5.process(reinterpret_cast<const uint8_t*>(input.data()), input.size());
    std::string hashHex = Botan::hex_encode(hashResult);
    std::transform(hashHex.begin(), hashHex.end(), hashHex.begin(), ::toupper);
    return hashHex;
    
}


