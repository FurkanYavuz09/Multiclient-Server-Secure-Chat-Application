// client.cpp
#include "client.h"

Client::Client(const char *serverIp, int port) : serverIp(serverIp), port(port), clientSocket(0), file("../multiclientmodel/logs/clientlog.txt"), cypher() {
    struct sockaddr_in serv_addr;

    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, serverIp, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(clientSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }
}

void Client::sendMessage(const char *message) {
    send(clientSocket, message, strlen(message), 0);
}

int Client::receiveMessage(char* buffer) {
    return read(clientSocket, buffer, 1024);
    
}
Client::~Client() {
    close(clientSocket);
}


