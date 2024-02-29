#include "fileIO.h"
#include <jsoncpp/json/json.h>

FileIO::FileIO(const std::string& filename) {
    fileStream.open(filename, std::ios::in | std::ios::out | std::ios::app);
    if (!fileStream.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }

}

void FileIO::write(const std::string& message) {
    if (fileStream.is_open())
    {
        std::time_t currentTime = std::time(nullptr);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
        fileStream << "[" << timestamp << "] " << message << std::endl;
    }
}

std::vector<std::string> FileIO::read() {
    
    std::vector<std::string> lines;
    if (fileStream.is_open()) {
        std::string line;
        while (std::getline(fileStream, line)) {
            lines.push_back(line);
        }
    }
    return lines;
}

std::string FileIO::strToJSON(const std::string& str) {
// Convert string to JSON
    Json::Value jsonMessage;
    jsonMessage["message"] = str;

    // Convert JSON to string
    Json::StreamWriterBuilder builder;
    std::string jsonStr = Json::writeString(builder, jsonMessage);
    return jsonStr;
}

FileIO::~FileIO() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
}


