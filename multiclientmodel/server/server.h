#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <algorithm>
#include "fileIO.h"
#include "cypher.h"

#define MAX_CLIENTS 10

class Server {
private:
    int port;
    int serverSocket;
    std::vector<int> clients;

public:
    FileIO file;
    Cypher cypher;
    Server(int port);
    void start();
    void handleClient(int clientSocket, std::string clientName);
    void broadcastMessage(int senderSocket, const char *message);
    void sendToClient(int clientSocket, const char *message);
    int receiveFromClient(char* buffer, int clientSocket);
    ~Server();
};

