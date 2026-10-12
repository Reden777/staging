//Compile: g++ -O3 -Wall -static sha3-512.cpp -o sha3-512.exe -lssl -lcrypto -lcrypt32 -ladvapi32 -lws2_32 -I./openssl/include

#include <iostream>
#include <fstream>
#include <string>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <algorithm>
#include <limits>
#include <iomanip>

// Function to read a file's content into a string
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

// Function to compute the SHA-3-512 hash using OpenSSL
std::string sha3_512_hash(const std::string& input) {
    unsigned char hash[SHA512_DIGEST_LENGTH];
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha3_512(), nullptr);
    EVP_DigestUpdate(ctx, input.c_str(), input.length());
    EVP_DigestFinal_ex(ctx, hash, nullptr);
    EVP_MD_CTX_free(ctx);

    std::ostringstream hashStream;
    for (int i = 0; i < SHA512_DIGEST_LENGTH; ++i) {
        hashStream << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return hashStream.str();
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
    
    std::string hash = sha3_512_hash(fileContents);
    // Convert the hash to uppercase
    std::transform(hash.begin(), hash.end(), hash.begin(), ::toupper);
    std::cout << "SHA-3-512 Hash: " << hash << std::endl;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Wait for the user to press Enter before quitting
    std::cout << "Press Enter to Quit...";
    std::cin.get();

    return 0;
}
