#include "client.h"
#include <thread>
#include <cryptopp/base64.h>

void sendit(Client& client){
    char buffer[1024];
    while (true) {
        std::string clientMessage = client.getName() + ": ";
        memset(buffer, 0, sizeof(buffer));
        std::cin.getline(buffer, 1024);
        clientMessage += buffer;
        if (strcmp(buffer, "exit") == 0) {
            exit(EXIT_SUCCESS);
            break;
        }
        std::cout << client.getName()+" : " << buffer << "\n";
        std::string ciphertext = client.cypher.encryptMessage(clientMessage);
        std::cout << "Encrypted Message: " << ciphertext << "\n";
        client.sendMessage( ciphertext.c_str());
    }
}
void receive(Client& client){
    char buffer[1024] ;
    int valread;
    while (true) {
        valread = client.receiveMessage(buffer);
        if (valread == 0) {
            std::cout << "Server disconnected." << std::endl;
            exit(EXIT_SUCCESS);
        }
        buffer[valread] = '\0'; // Null-terminate the received data
        std::cout << " Encrypted Message: " << buffer << "\n";
        std::string ciphertext = client.cypher.decryptMessage(buffer);
        std::cout << "Decrypted Message  " << ciphertext << "\n";
    }
}

void clientRun() {
    Client client("127.0.0.1", 8080);
    char buffer[1024];
    std::cout << "Please Enter Your Name: ";
    std::cin.getline(buffer, 1024);
    client.setName(buffer);
    client.sendMessage(client.getName().c_str());
    char keyAndIV[1024];
    int bytes_received = client.receiveMessage(keyAndIV);
    keyAndIV[bytes_received] = '\0';
    std::string keyAndIVStr(keyAndIV);// Extract key and IV from received data
    if (keyAndIVStr.size() < client.cypher.key.size() + client.cypher.iv.size()) {// Ensure that there's enough data to extract key and IV
        std::cerr << "Received data is not of expected size." << "\n";
        exit(0);
    }
    std::string keyStr = keyAndIVStr.substr(0, client.cypher.key.size());
    std::string ivStr = keyAndIVStr.substr(client.cypher.key.size(), client.cypher.iv.size());
    CryptoPP::SecByteBlock key(reinterpret_cast<const CryptoPP::byte*>(keyStr.data()), keyStr.size());
    CryptoPP::SecByteBlock iv(reinterpret_cast<const CryptoPP::byte*>(ivStr.data()), ivStr.size());
    client.cypher.key = key;
    client.cypher.iv = iv;
    // std::thread send_thread(sendit, std::ref(client));
    // std::thread receive_thread(receive, std::ref(client));
    std::thread send_thread([&client]() {
        sendit(client);
    });

    std::thread receive_thread([&client]() {
        receive(client);
    });
    
    send_thread.join();
    receive_thread.join();   
}

int main() {
     clientRun();
     return 0;
}