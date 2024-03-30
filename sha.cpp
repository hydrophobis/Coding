#include <iostream>
#include <string>
#include <sstream>
#include <openssl/sha.h>
#include <iomanip>

std::string sha256(const std::string &str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <string_to_hash>\n";
        return 1;
    }

    std::string input = argv[1];
    std::string hashed = sha256(input);
    std::cout << "SHA256 hash of '" << input << "': " << hashed << std::endl;

    return 0;
}
