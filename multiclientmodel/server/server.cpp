#include "server.h"
#include <fileIO.h>
#include <cypher.h>
#include <cryptopp/base64.h>

Server::Server(int port) : port(port), serverSocket(0), file("../multiclientmodel/logs/serverlog.txt"), cypher(){
    struct sockaddr_in address;
    int opt = 1;

    // Creating socket file descriptor
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        file.write("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        file.write("Error setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Forcefully attaching socket to the port
    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        file.write("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, MAX_CLIENTS) < 0) {
        perror("listen");
        file.write("Conneclisten");
        exit(EXIT_FAILURE);
    }
}
void Server::start() {
    int clientSocket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    using namespace CryptoPP;
    AutoSeededRandomPool prng;
    prng.GenerateBlock(cypher.key, cypher.key.size());
    prng.GenerateBlock(cypher.iv, cypher.iv.size());
    // std::cout << "key: "<< cypher.key.data() << std::endl;
    // std::cout << "iv: "<< cypher.iv.data() << std::endl;
    // std::cout << "key : ";
    // cypher.encoder.Put(cypher.key.data(), cypher.key.size());
    // cypher.encoder.MessageEnd();
    // std::cout << "\n";
    // std::cout << "iv : ";
    // cypher.encoder.Put(cypher.iv.data(), cypher.iv.size());
    // cypher.encoder.MessageEnd();
    // std::cout << "\n";
    std::string keyAndIV(reinterpret_cast<const char*>(cypher.key.data()), cypher.key.size());
    keyAndIV.append(reinterpret_cast<const char*>(cypher.iv.data()), CryptoPP::AES::BLOCKSIZE);

    while (true) {

        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            file.write("Connection Accepted");
            exit(EXIT_FAILURE);
        }
        char clientName[1024] ;
        receiveFromClient(clientName, clientSocket);
        std::string name(clientName);
        std::cout << name  <<" entered the chat... " << "\n";
        memset(clientName, 0, sizeof(clientName));

        sendToClient(clientSocket, keyAndIV.c_str());

        // Handle each client connection in a separate thread
        std::thread clientThread(&Server::handleClient, this, clientSocket, name);
        clientThread.detach(); // Detach the thread to run independently
    }
}

void Server::handleClient(int clientSocket, std::string clientName) {
    char buffer[1024] ;
    int valread;
    clients.push_back(clientSocket);
    std::string leftMessage = clientName+" left from chat... ";
    file.write(leftMessage);
    while ((valread = read(clientSocket, buffer, 1024)) > 0) {
        buffer[valread] = '\0';
        
        std::cout << "Encrypted Message: " << buffer << "\n";
        std::string decrypted = cypher.decryptMessage(buffer);
        std::cout <<"Decrypted Message  "<< decrypted << "\n";
        broadcastMessage(clientSocket, buffer);
        file.write(decrypted);
        memset(buffer, 0, sizeof(buffer));
    }
    
    if (valread == 0) {
        std::string encryptedLeftMessage = cypher.encryptMessage(leftMessage);
        broadcastMessage(clientSocket, encryptedLeftMessage.c_str());
        std::cout << cypher.decryptMessage(encryptedLeftMessage) << "\n";
    } 
    else {
        perror("read");
    }

    clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
    close(clientSocket);
}

void Server::broadcastMessage(int senderSocket, const char *message) {
    
    for (int clientSocket : clients) {
            if (clientSocket != senderSocket) {
                send(clientSocket, message, strlen(message), 0);
            }
    }
}

void Server::sendToClient(int clientSocket, const char *message) {
    send(clientSocket, message, strlen(message), 0);
}

int Server::receiveFromClient(char* buffer, int clientSocket) {
    return read(clientSocket, buffer, 1024);
    
}

Server::~Server() {
    close(serverSocket);
}