#include <fileIO.h>
#include <thread>
#include "server.h"
#include "encryption/cypher.h"
#include <cryptopp/secblock.h>

void receiveMessages(Server& server, FileIO& file, Cypher& cypher) {
    char buffer[1024];
    while (true) {
        // Receive message from client
        int bytes_received = server.receiveFromClient(buffer);
        if (bytes_received <= 0) {
            std::cerr << "Error: Connection closed by client.\n";
            break;
        }
        buffer[bytes_received] = '\0';
       
        std::cout << "Encrypted Client Message : " << buffer << std::endl;

        std::string ciphertext = cypher.decryptMessage(buffer);

       
        std::cout << "Decrypted Client Message : " << ciphertext << std::endl;

        file.write(ciphertext);

    }
}

void sendMessages(Server& server,  FileIO& file, Cypher& cypher){
    char buffer[1024];
    while (true) {
        // Send message to client
        
        std::cin.getline(buffer, 1024);
       
        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        std::string jsontext = file.strToJSON(buffer);
        
        std::cout << "Original Server Message: " << jsontext << std::endl;
        std::string ciphertext = cypher.encryptMessage(jsontext);
        server.sendToClient(ciphertext.c_str());

        file.write(buffer);
    }
}

int main() {
    Server server;
 
    FileIO file("../logs/serverlog.txt");

    Cypher cypher;
    using namespace CryptoPP;
    AutoSeededRandomPool prng;
    prng.GenerateBlock(cypher.key, cypher.key.size());
    prng.GenerateBlock(cypher.iv, cypher.iv.size());

    // std::cout << "key: "<< cypher.key.data() << std::endl;
    // std::cout << "iv: "<< cypher.iv.data() << std::endl;
    std::cout << "key : ";
    cypher.encoder.Put(cypher.key.data(), cypher.key.size());
    cypher.encoder.MessageEnd();
    std::cout << "\n";

    std::cout << "iv : ";
    cypher.encoder.Put(cypher.iv.data(), cypher.iv.size());
    cypher.encoder.MessageEnd();
    std::cout << "\n";

    std::string keyAndIV(reinterpret_cast<const char*>(cypher.key.data()), cypher.key.size());
    keyAndIV.append(reinterpret_cast<const char*>(cypher.iv.data()), CryptoPP::AES::BLOCKSIZE);

    server.acceptConnection();
    
    server.sendToClient(keyAndIV.c_str());

    std::cout << "Server connected to client. Start chatting!\n";

    std::thread receive_thread(receiveMessages, std::ref(server) , std::ref(file), std::ref(cypher));

    // // std::thread receive_thread([&](Server server, FileIO file){
    // //     receiveMessages(server, file);
    // // });

    std::thread send_thread(sendMessages, std::ref(server), std::ref(file), std::ref(cypher));

    receive_thread.join();
    send_thread.join();

    server.closeConnection();

    return 0;
}
