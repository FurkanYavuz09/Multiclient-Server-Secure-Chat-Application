#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>

using namespace std;

class Server {
private:
    int server_socket;
    int client_socket;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_addr_size;
    const int PORT;
    const int BACKLOG;
    const int BUFFER_SIZE;
    
    // vector<int> client_sockets;

public:
    Server(int port = 54000, int backlog = 5, int buffer_size = 1024);
    void acceptConnection();
    void sendToClient(const char* message);
    int receiveFromClient(char* buffer);
    void closeConnection();
    int getPort();
    // void sendToAllClients(const char* message);
    ~Server();
};
