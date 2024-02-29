#include "client.h"
#include "fileIO.h"
#include <thread>
#include "encryption/cypher.h"
#include <cryptopp/secblock.h>

void receiveMessages(Client& client, FileIO& file, Cypher& cypher) {
    char buffer[1024];
    while (true) {
        // Receive message from server
        int bytes_received = client.receiveFromServer(buffer);
        if (bytes_received == -1) {
            std::cerr << "Error: Failed to receive message from server.\n";
        } else if (bytes_received == 0) {
            std::cerr << "Connection closed by server.\n";
            break;
        } else {
            buffer[bytes_received] = '\0';
            std::cout << "Received message from server: " << buffer << '\n';
        }
        std::cout << "Received Encrypted Server Message: " << buffer << "\n";
        std::string ciphertext = cypher.decryptMessage(buffer);
        std::cout << "Decrypted Server Message: " << ciphertext << "\n";
        
        file.write(ciphertext);
    }
}

void sendMessages(Client& client, FileIO& file, Cypher& cypher) {
    char buffer[1024];
    while (true) {
        // Send message to server
        std::cin.getline(buffer, 1024);
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
        std::cout << "Original Client Message: " << buffer << std::endl;
        std::string ciphertext = cypher.encryptMessage(buffer);
        std::cout << "Encrypted Client Message: " << ciphertext << "\n";

        client.sendToServer(ciphertext.c_str());
        
        file.write(buffer);
    }
}

int main() {
    
    Client client;

    FileIO file("../logs/clientlog.txt");

    Cypher cypher;

    char keyAndIV[1024];
    int bytes_received = client.receiveFromServer(keyAndIV);

    keyAndIV[bytes_received] = '\0';

    // Extract key and IV from received data
    std::string keyAndIVStr(keyAndIV);
    std::cout << "Received key and IV: " << keyAndIVStr << "\n";
    // Ensure that there's enough data to extract key and IV
    if (keyAndIVStr.size() < cypher.key.size() + cypher.iv.size()) {
        std::cerr << "Received data is not of expected size." << "\n";
        return 1;
    }
    std::string keyStr = keyAndIVStr.substr(0, cypher.key.size());
    std::string ivStr = keyAndIVStr.substr(cypher.key.size(), cypher.iv.size());

    CryptoPP::SecByteBlock key(reinterpret_cast<const CryptoPP::byte*>(keyStr.data()), keyStr.size());
    CryptoPP::SecByteBlock iv(reinterpret_cast<const CryptoPP::byte*>(ivStr.data()), ivStr.size());

    cypher.key = key;
    cypher.iv = iv;

    std::cout << "key : ";
    cypher.encoder.Put(cypher.key.data(), cypher.key.size());
    cypher.encoder.MessageEnd();
    std::cout << "\n";

    std::cout << "iv : ";
    cypher.encoder.Put(cypher.iv.data(), cypher.iv.size());
    cypher.encoder.MessageEnd();
    std::cout << "\n";
  
    std::thread receive_thread(receiveMessages, std::ref(client), std::ref(file), std::ref(cypher));
    std::thread send_thread(sendMessages, std::ref(client), std::ref(file), std::ref(cypher));

    receive_thread.join();
    send_thread.join();

    return 0;
}

