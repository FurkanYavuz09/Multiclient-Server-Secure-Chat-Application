// server_main.cpp
#include "server.h"
#include <thread>
#include "fileIO.h"

void messageSender(Server& server) {
    char message[1024];
    while (true) {
        
        std::string clientMessage = "[SERVER]: ";

        std::cin.getline(message, 1024);
        if (strcmp(message, "exit") == 0) {
            break;
        }
        
        int  a  = -1;

        std::string jsontext = server.file.strToJSON(message);
        clientMessage += jsontext;
        
        std::cout << jsontext << std::endl;
        std::string ciphertext = server.cypher.encryptMessage(clientMessage);

        server.broadcastMessage(a, ciphertext.c_str());
        std::cout << "Encrypted Message sent!" << std::endl;
        server.file.write(clientMessage);
    }
}

int main() {
    
    Server server(8080);

    std::cout << "Server started ..." << std::endl;
    // Start the server in a separate thread
    std::thread serverThread(&Server::start, &server);
    
    // Start the message sending loop in the main thread
    messageSender(server);

    // Join the server thread to keep the main thread alive
    serverThread.join();

    return 0;
}
