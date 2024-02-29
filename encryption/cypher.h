#pragma once

#include <iostream>
#include <string>
#include "cryptopp/cryptlib.h"
#include "cryptopp/rijndael.h"
#include "cryptopp/modes.h"
#include "cryptopp/files.h"
#include "cryptopp/osrng.h"
#include "cryptopp/hex.h"

class Cypher {

public:
    Cypher();
    std::string encryptMessage(const std::string& message);
    std::string decryptMessage(const std::string& message);
    CryptoPP::SecByteBlock key;
    CryptoPP::SecByteBlock iv;
    CryptoPP::HexEncoder encoder;
};