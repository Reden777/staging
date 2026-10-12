//Compile: g++ -O3 -Wall -static New_Multi-Hash.cpp -o New_Multi-Hash.exe -lssl -lcrypto -lcrypt32 -ladvapi32 -lws2_32 -I./openssl/include
//This differs from the original code in that it doesn't uppercase each hash iteration. It maintains a greater purity in the final hash. It likes traditional programming with maintaining hash purity.
//The final hash can be uppercase. That doesn't seem to make a difference.
//Based on Multi-Hash_OpenSSL_SHA3-512_Preserve_Original.cpp


#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <openssl/evp.h>
#include <iomanip>
#include <sstream>
#include <limits>

// Function to compute SHA-3 512 hash using OpenSSL EVP API
std::string sha3_512_hash(const std::string& input) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        return "";
    }

    if (!EVP_DigestInit_ex(ctx, EVP_sha3_512(), nullptr)) {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    if (!EVP_DigestUpdate(ctx, input.c_str(), input.length())) {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    if (!EVP_DigestFinal_ex(ctx, hash, &hash_len)) {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    EVP_MD_CTX_free(ctx);

    std::string hash_str;
    for (unsigned int i = 0; i < hash_len; ++i) {
        char buf[3];
        sprintf(buf, "%02x", hash[i]);
        hash_str += buf;
    }

    return hash_str;
}

bool isFilename(const std::string& input) {
    size_t len = input.length();
    return (len >= 4 && (input[len - 4] == '.' || input[len - 3] == '.'));
}

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

// Updated function to truncate SHA-3 512 to 64-bit
std::string truncate_to_64_bit(const std::string& hash512) {
    // Each 64-bit group is 16 hex characters
    unsigned long long group1 = std::stoull(hash512.substr(0, 16), nullptr, 16);
    unsigned long long group2 = std::stoull(hash512.substr(16, 16), nullptr, 16);
    unsigned long long group3 = std::stoull(hash512.substr(32, 16), nullptr, 16);
    unsigned long long group4 = std::stoull(hash512.substr(48, 16), nullptr, 16);
    unsigned long long group5 = std::stoull(hash512.substr(64, 16), nullptr, 16);
    unsigned long long group6 = std::stoull(hash512.substr(80, 16), nullptr, 16);
    unsigned long long group7 = std::stoull(hash512.substr(96, 16), nullptr, 16);
    unsigned long long group8 = std::stoull(hash512.substr(112, 16), nullptr, 16);

    // Add the eight 64-bit groups together
    unsigned long long combined_64_bit = group1 + group2 + group3 + group4 + group5 + group6 + group7 + group8;

    // Since we are working with 64-bit integers, the modulo operation is inherent
    // We directly convert the result to a hex string
    std::stringstream ss;
    ss << std::hex << std::uppercase << (combined_64_bit & 0xFFFFFFFFFFFFFFFFULL);
    return ss.str();
}

std::string countLinesInFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "Error: Could not open the file.";
    }

    long lineCount = 0;
    std::string line;
    while (std::getline(file, line)) {
        ++lineCount;
    }

    file.close();

    // Convert the line count to a string
    std::ostringstream oss;
    oss << lineCount;
    return oss.str();
}

int main() {
    std::cout << "Multi-Hashing SHA-3 512 Preserve Original by Anthro Teacher." << std::endl;
    std::cout << "This will repeat your Text a specified number of times." << std::endl;
    std::cout << "It will hash this repeated text repeatedly a specified number of times." << std::endl;
    std::cout << "Hashing more times may make the text even more potent." << std::endl;
    std::cout << "This preserves the orginial text in each hashing step." << std::endl << std::endl;

    std::string original;
    std::cout << "Enter Text or Filename: ";
    std::getline(std::cin, original);

    long times_to_repeat = 1;
    std::string str_times_to_repeat = "";
    std::cout << "Number of times to Repeat [1-10000000] (or MAX): ";
    std::cin >> str_times_to_repeat;
    std::cin.ignore(); // Ignore the newline character
    if (str_times_to_repeat == "MAX" || str_times_to_repeat == "max" || str_times_to_repeat == "Max") {
        str_times_to_repeat = "10000000";
    }

    times_to_repeat = std::stol(str_times_to_repeat);

    long num_hash_levels;
    std::string str_num_hash_levels = "";
    std::cout << "Number of Hash Levels [1-1000]: (or MAX): ";
    std::cin >> str_num_hash_levels;
    std::cin.ignore(); // Ignore the newline character

    if (str_num_hash_levels == "MAX" || str_num_hash_levels == "max" || str_num_hash_levels == "Max"){
        str_num_hash_levels = "1000";
    }

    num_hash_levels = std::stol(str_num_hash_levels);

    std::string truncate_choice;
    std::cout << "Truncate to 64-Bit: (Y/n): ";
    std::getline(std::cin, truncate_choice);

    bool truncate_to_64 = (truncate_choice == "Y" || truncate_choice == "y");

    std::string line;
    int linenum = 1;
    std::string repeated_text = "";
    std::string hashed_text = "";
    
    if (isFilename(original)) {
        std::string linecount = countLinesInFile(original);
 
        std::ifstream file(original);
        std::ofstream outFile("codes.txt");
        std::string repeated_hash = "";
        std::string original_text;
        while (std::getline(file, line)) {
            original_text = line;
            repeated_text.clear(); // Reset repeated_text to an empty string
            for (long i = 0; i < times_to_repeat; ++i) {
                repeated_text += line + "\n";
            }
            hashed_text = sha3_512_hash(repeated_text);
            //std::transform(hashed_text.begin(), hashed_text.end(), hashed_text.begin(), ::toupper);
            for (long level = 1; level <= num_hash_levels; ++level) {
                std::cout << "Calculating Hashing Level " << level << ": Line #" << linenum << "/" + linecount + " writing.\r";
                repeated_hash.clear(); // Reset repeated_hash to an empty string
                for (int i = 0; i < times_to_repeat; ++i) {
                    repeated_hash += hashed_text + "\n";
                }
                
                hashed_text = sha3_512_hash(repeated_hash + ": " + original_text);
                //std::transform(hashed_text.begin(), hashed_text.end(), hashed_text.begin(), ::toupper);
            }

            if (truncate_to_64) {
                hashed_text = truncate_to_64_bit(hashed_text) +  + ": " + original_text;
            }

            //Only uppercase at the end
            std::transform(hashed_text.begin(), hashed_text.end(), hashed_text.begin(), ::toupper);

            outFile << line << ": " << hashed_text << std::endl;
            linenum += 1;
            //std::cout << ": Line #" << linenum << "/" + linecount + " written.\r";
        }
        std::cout << std::endl;

        file.close();
        outFile.close();
        std::cout << "Codes written to codes.txt" << std::endl;
    } else {
        std::transform(hashed_text.begin(), hashed_text.end(), hashed_text.begin(), ::toupper);
        for (int i = 0; i < times_to_repeat; ++i) {
            repeated_text += original + "\n";
        }
        hashed_text = sha3_512_hash(repeated_text + ": " + original);
        //std::transform(hashed_text.begin(), hashed_text.end(), hashed_text.begin(), ::toupper);
        
        for (int level = 1; level <= num_hash_levels; ++level) {
            std::cout << "Hashing Level " << level << "\r";
            
            std::string repeated_hash;
            for (int i = 0; i < times_to_repeat; ++i) {
                repeated_hash += hashed_text + "\n";
            }
            
            hashed_text = sha3_512_hash(repeated_hash + ": " + original);
            repeated_hash.clear();
            //std::transform(hashed_text.begin(), hashed_text.end(), hashed_text.begin(), ::toupper);
        }

        if (truncate_to_64) {
            hashed_text = truncate_to_64_bit(hashed_text);
        }

        // Only uppercase at the end.
        std::transform(hashed_text.begin(), hashed_text.end(), hashed_text.begin(), ::toupper);

        std::cout << "\n[Repeats: " << times_to_repeat << "] (Hash level: " << num_hash_levels << "): " << hashed_text << std::endl;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Wait for the user to press Enter before quitting
    std::cout << "Press Enter to Quit...";
    std::cin.get();

    return 0;
}
