// client_main.cpp
#include "client.h"
#include <thread>
#include <cryptopp/base64.h>

void sendit(Client& client){

    char buffer[1024];
    while (true) {
        // Send message to server
        std::string clientMessage = client.getName() + ": ";
        std::cin.getline(buffer, 1024);
        clientMessage += buffer;
        if (strcmp(buffer, "exit\0") == 0) {
            break;
        }
        std::cout << "Original Message: " << buffer << std::endl;
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

        std::cout << "Received Encrypted Server Message: " << buffer << "\n";
        std::string ciphertext = client.cypher.decryptMessage(buffer);
        std::cout << "Decrypted Message From " << ciphertext << "\n";
    }
}

int main() {
    Client client("127.0.0.1", 8080);

    char buffer[1024];
    std::cout << "enter your name: ";
    std::cin.getline(buffer, 1024);
    client.setName(buffer);

    char keyAndIV[1024];
    int bytes_received = client.receiveMessage(keyAndIV);

    keyAndIV[bytes_received] = '\0';

    // Extract key and IV from received data
    std::string keyAndIVStr(keyAndIV);
    // std::cout << "Received key and IV: " << keyAndIVStr << "\n";

    // Ensure that there's enough data to extract key and IV
    if (keyAndIVStr.size() < client.cypher.key.size() + client.cypher.iv.size()) {
        std::cerr << "Received data is not of expected size." << "\n";
        return 1;
    }
    std::string keyStr = keyAndIVStr.substr(0, client.cypher.key.size());
    std::string ivStr = keyAndIVStr.substr(client.cypher.key.size(), client.cypher.iv.size());

    CryptoPP::SecByteBlock key(reinterpret_cast<const CryptoPP::byte*>(keyStr.data()), keyStr.size());
    CryptoPP::SecByteBlock iv(reinterpret_cast<const CryptoPP::byte*>(ivStr.data()), ivStr.size());

    client.cypher.key = key;
    client.cypher.iv = iv;

    // std::cout << "key : ";
    // client.cypher.encoder.Put(client.cypher.key.data(), client.cypher.key.size());
    // client.cypher.encoder.MessageEnd();
    // std::cout << "\n";

    // std::cout << "iv : ";
    // client.cypher.encoder.Put(client.cypher.iv.data(), client.cypher.iv.size());
    // client.cypher.encoder.MessageEnd();
    // std::cout << "\n";


    std::thread send_thread(sendit, std::ref(client));
    std::thread receive_thread(receive, std::ref(client));

    send_thread.join();
    receive_thread.join();

    return 0;
}
