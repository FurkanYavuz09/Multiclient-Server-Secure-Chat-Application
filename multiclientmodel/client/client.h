// client.h
#pragma once

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "fileIO.h"
#include "cypher.h"


class Client {
private:
    const char *serverIp;
    int port;
    int clientSocket;
    std::string name;

public:
    // FileIO file;
    Cypher cypher;
    Client(const char *serverIp, int port);
    ~Client();
    void sendMessage(const char *message);
    int receiveMessage(char* buffer);
    int getClientSocket() { return clientSocket; }
    std::string getName() { return name; }
    void setName(std::string name) { this->name = name; }
};


