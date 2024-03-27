// 2.1 Million Guesses Per Second
// aid! 0.260845 in 5.7 Million
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace chrono;

// Function to generate SHA256 hash of a string
string sha256(const string& str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.length());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

// Function to generate combinations using recursive backtracking
void generateCombinations(const string& charSet, int maxLength, const string& password, string currentString, int& guesses) {
    static auto start = steady_clock::now();

    if (sha256(currentString) == password) {
        auto end = steady_clock::now();
        double elapsed_seconds = duration<double>(end - start).count();
        cout << "Found " << password << "(" << currentString << ")" << " with " << guesses << " guesses" << " in " << elapsed_seconds << " seconds" << endl;
        exit(0);
    }

    cout << sha256(currentString) << "\n";

    for (char c : charSet) {
        currentString.push_back(c);
        ++guesses;

        if (currentString.length() <= maxLength) {
            generateCombinations(charSet, maxLength, password, currentString, guesses);
        }

        currentString.pop_back();
    }
}

int main() {
    string charSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+[]{}|,'.<>?~";

    string password;
    int maxLength;

    cout << "Hashed Input Password: ";
    cin >> password;

    cout << "Length Before Hashing: ";
    cin >> maxLength;

    string currentString;
    int guesses = 0;

    generateCombinations(charSet, maxLength, password, currentString, guesses);

    return 0;
}
