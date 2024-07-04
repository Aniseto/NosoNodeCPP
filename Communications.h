#pragma once
#include <string>
#include <boost/asio.hpp> //Boost Library //vcpkg install boost:x64-windows-static https://www.boost.org/
#include <boost/array.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
//#include <>

#ifndef _WIN32
#include<sys/socket.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define WSACleanup() void()
#define closesocket(s) close(s)
#define SOCKET_ERROR -1
#endif

std::string SendStringToNode(const std::string& ip, int port, const std::string& message) {
#ifdef _WIN32
    WSADATA wsaData; // Wsdata Variable
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Error Initializing Winsock." << std::endl;
        return "Error Initializing Winsock.";
    }
#endif

    std::cout << "\nConnecting to " << ip << ":" << port << std::endl;

    // Create TCP socket
    SOCKET socketCliente = socket(AF_INET, SOCK_STREAM, 0);
    if (socketCliente == INVALID_SOCKET) {
        std::cout << "Error creating socket." << std::endl;
        WSACleanup();
        return "Error creating socket.";
    }

    // Specify Address and port to connect
    sockaddr_in direccionServidor{};
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &(direccionServidor.sin_addr)) <= 0) {
        std::cout << "Error converting IP address." << std::endl;
        closesocket(socketCliente);
        WSACleanup();
        return "Error converting IP address.";
    }

    //*! Connect to server
    if (connect(socketCliente, reinterpret_cast<struct sockaddr*>(&direccionServidor), sizeof(direccionServidor)) == SOCKET_ERROR) {
        std::cout << "Error connecting to server." << std::endl;
        closesocket(socketCliente);
        WSACleanup();
        return "Error connecting to server.";
    }

    // Send message to server
    if (send(socketCliente, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
        std::cout << "Error sending message." << std::endl;
        closesocket(socketCliente);
        WSACleanup();
        return "Error sending message.";
    }

    if (message == "GETZIPSUMARY\n")
    {
        std::string response = "Sumary Downloaded\n";
        std::remove("summary.zip");
        std::ofstream outputFile("summary.zip", std::ios::binary);
        char buffer[1024];
        std::string respuesta;
        int bytesRecibidos;

        while ((bytesRecibidos = recv(socketCliente, buffer, sizeof(buffer) - 1, 0)) > 0)
        {
            // Write Bytes to file
            outputFile.write(buffer, bytesRecibidos);
        }

        outputFile.close();

        return response;
    }

    char buffer[1024];
    std::string respuesta;
    int bytesRecibidos;
    while ((bytesRecibidos = recv(socketCliente, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesRecibidos] = '\0';
        respuesta += buffer;
    }

    // Close Connection and return answer
    closesocket(socketCliente);
    WSACleanup();
    return respuesta;

}
std::string GetUTCTimeFromNTPServer(const std::string& ntpServer = "pool.ntp.org") 
{
    boost::asio::io_context io_context;
    boost::asio::ip::udp::resolver resolver(io_context);
    boost::asio::ip::udp::endpoint receiver_endpoint = *resolver.resolve(boost::asio::ip::udp::v4(), ntpServer, "ntp").begin();
    boost::asio::ip::udp::socket socket(io_context);
    socket.open(boost::asio::ip::udp::v4());

    // Formar la solicitud NTP
    std::array<uint8_t, 48> send_buf{ 0 };
    send_buf[0] = 0x1B;

    // Enviar la solicitud NTP
    socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

    // Recibir la respuesta NTP
    std::array<uint8_t, 2048> recv_buf;
    boost::asio::ip::udp::endpoint sender_endpoint;
    size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

    // Analizar la respuesta NTP
    uint64_t timestamp = (recv_buf[40] << 24) | (recv_buf[41] << 16) | (recv_buf[42] << 8) | recv_buf[43];
    uint64_t epoch = timestamp - 2208988800UL; // Diferencia entre 1900-1970
    std::time_t current_time = static_cast<std::time_t>(epoch);

    // Convertir la hora UTC en una cadena usando std::to_string
    return std::to_string(current_time);
}

std::string GetUTCTimeString(const std::string& ntpServer = "pool.ntp.org")
{
    try
    {
        boost::asio::io_context io_context;
        boost::asio::ip::udp::resolver resolver(io_context);
        boost::asio::ip::udp::endpoint receiver_endpoint = *resolver.resolve(boost::asio::ip::udp::v4(), ntpServer, "ntp").begin();
        boost::asio::ip::udp::socket socket(io_context);
        socket.open(boost::asio::ip::udp::v4());

        // Formar la solicitud NTP
        std::array<uint8_t, 48> send_buf{ 0 };
        send_buf[0] = 0x1B;

        // Enviar la solicitud NTP
        socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

        // Recibir la respuesta NTP
        std::array<uint8_t, 48> recv_buf;
        boost::asio::ip::udp::endpoint sender_endpoint;
        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

        if (len < 48)
        {
            throw std::runtime_error("Respuesta NTP incompleta");
        }

        // Analizar la respuesta NTP
        uint32_t seconds = (recv_buf[40] << 24) | (recv_buf[41] << 16) | (recv_buf[42] << 8) | recv_buf[43];
        uint64_t epoch = static_cast<uint64_t>(seconds) - 2208988800U; // Diferencia entre 1900-1970

        socket.close();

        return std::to_string(epoch);
    }
    catch (const std::exception& ex)
    {
        // Manejo básico de errores
        throw std::runtime_error(std::string("Error: ") + ex.what());
    }
}
