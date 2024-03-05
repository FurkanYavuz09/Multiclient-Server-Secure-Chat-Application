#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class Client {
private:
    int client_socket;
    struct sockaddr_in server_addr;
    const char* SERVER_IP;
    const int PORT;
    const int BUFFER_SIZE;

public:
    Client(const char* server_ip = "127.0.0.1", int port = 54000, int buffer_size = 1024);
    void sendToServer(const char* message);
    int receiveFromServer(char* buffer);
    ~Client();
};
