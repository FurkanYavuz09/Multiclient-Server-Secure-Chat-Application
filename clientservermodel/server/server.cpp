#include "server.h"
#include <vector>

Server::Server(int port, int backlog, int buffer_size) : PORT(port), BACKLOG(backlog), BUFFER_SIZE(buffer_size) {
    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Error: Could not create socket.\n";
        exit(EXIT_FAILURE);
    }


    
    // std::vector<int> client_sockets;



    std::cout << "Server socket created\n";
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error: Could not bind socket to address.\n";
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, BACKLOG) == -1) {
        std::cerr << "Error: Could not listen for connections.\n";
        exit(EXIT_FAILURE);
    }
}

void Server::acceptConnection() {
    client_addr_size = sizeof(client_addr);



    if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size)) == -1) {        std::cerr << "Error: Could not accept connection.\n";
        exit(EXIT_FAILURE);
    }
    // client_sockets.push_back(client_socket);
}

void Server::sendToClient(const char* message) {
    send(client_socket, message, strlen(message), 0);
}

int Server::receiveFromClient(char* buffer) {
    return recv(client_socket, buffer, BUFFER_SIZE, 0);
}

void Server::closeConnection() {
    close(client_socket);
}




Server::~Server() {
   
    close(server_socket);
}

int Server::getPort() {
    return PORT;
}
