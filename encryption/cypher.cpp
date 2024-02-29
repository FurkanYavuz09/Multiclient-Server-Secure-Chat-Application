#include <iostream>
#include <string>
#include "cypher.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/rijndael.h"
#include "cryptopp/modes.h"
#include "cryptopp/files.h"
#include "cryptopp/osrng.h"
#include "cryptopp/hex.h"
#include <cryptopp/base64.h>


Cypher::Cypher( ): key(CryptoPP::AES::DEFAULT_KEYLENGTH), iv(CryptoPP::AES::BLOCKSIZE),encoder(new CryptoPP::FileSink(std::cout)){
    
}

std::string Cypher:: encryptMessage(const std::string& message) {
    
    using namespace CryptoPP;

    std::string cipher;
    std::string encoded;

    // std::cout << "plain text: " << message << std::endl;

    try
    {
        CBC_Mode< AES >::Encryption e;
        e.SetKeyWithIV(key, key.size(), iv);

        
        StringSource s(message, true, 
            new StreamTransformationFilter(e,
                new StringSink(cipher)
            ) // StreamTransformationFilter
        ); // StringSource

        StringSource(cipher, true,
            new Base64Encoder(
                new StringSink(encoded)
            )
        );
    }
    catch(const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    // std::cout << "key: ";
    // encoder.Put(key, key.size());
    // encoder.MessageEnd();
    // std::cout << std::endl;

    // std::cout << "iv: ";
    // encoder.Put(iv, iv.size());
    // encoder.MessageEnd();
    // std::cout << std::endl;

    // std::cout << "encrypted text: ";
    // encoder.Put((const byte*)&cipher[0], cipher.size());
    // encoder.MessageEnd();
    // std::cout << std::endl;
    return encoded;
}

std::string Cypher:: decryptMessage(const std::string& ciphertext) {
    std::string recovered ;
    std::string decoded;

    using namespace CryptoPP;

    try
    {
        CBC_Mode< AES >::Decryption d;
        d.SetKeyWithIV(key, key.size(), iv);

        CryptoPP::StringSource(ciphertext, true,
        new CryptoPP::Base64Decoder(
            new CryptoPP::StringSink(decoded)
        )
    );


        StringSource s(decoded, true, 
            new StreamTransformationFilter(d,
                new StringSink(recovered)
            ) // StreamTransformationFilter
        ); // StringSource

        
        // std::cout << "recovered text: " << recovered << std::endl;
    }
    catch(const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    return recovered;

}

