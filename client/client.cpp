#include "client.h"

Client::Client(const char* server_ip, int port, int buffer_size) : SERVER_IP(server_ip), PORT(port), BUFFER_SIZE(buffer_size) {
    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Error: Could not create socket.\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Client socket created\n";
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error: Could not connect to server.\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected to server\n";
}

void Client::sendToServer(const char* message) {
    send(client_socket, message, strlen(message), 0);
}

int Client::receiveFromServer(char* buffer) {
    return recv(client_socket, buffer, BUFFER_SIZE, 0);
}

Client::~Client() {
    close(client_socket);
}
