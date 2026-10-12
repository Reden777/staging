#include <iostream>
#include <fstream>
#include <string>
#include "picosha2.h" // Include the picosha2 library
#include <limits>

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        return "";
    }
    
    std::string fileContents;
    file.seekg(0, std::ios::end);
    fileContents.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&fileContents[0], fileContents.size());
    file.close();
    
    return fileContents;
}

int main(int argc, char* argv[]) {
    std::string filePath;
    
    if (argc > 1) {
        filePath = argv[1];
    } else {
        std::cout << "Enter the file path: ";
        std::getline(std::cin, filePath);
    }
    
    std::string fileContents = readFile(filePath);
    while (fileContents.empty()) {
        std::cout << "File not found. Enter the file path: ";
        std::getline(std::cin, filePath);
        fileContents = readFile(filePath);
    }
    
    std::string hash = picosha2::hash256_hex_string(fileContents);
    // Convert the hash to uppercase
    std::transform(hash.begin(), hash.end(), hash.begin(), ::toupper);
    std::cout << "SHA-256 Hash: " << hash << std::endl;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Wait for the user to press Enter before quitting
    std::cout << "Press Enter to Quit...";
    std::cin.get();

    return 0;
}