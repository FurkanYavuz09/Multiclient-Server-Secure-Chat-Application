#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

class FileIO {
private:
    std::fstream fileStream;

public:
    FileIO(const std::string& filename);

    void write(const std::string& message);

    std::vector<std::string> read();

    std::string strToJSON(const std::string& str);

    ~FileIO();
};
