#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <openssl/sha.h>
#include <thread>
#include <atomic>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace chrono;

bool debug = false;

string sha256(const string &str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);

    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    return ss.str();
}

void attemptCombination(const string &charSet, int maxLength, const string &passwordHash, string currentString, atomic<bool> &found, atomic<long long> &guesses) {
    if (found) return;

    if (currentString.size() == maxLength) {
        if (sha256(currentString) == passwordHash) {
            found = true;
            cout << "\nFound " << passwordHash << "(" << currentString << ")" << " with " << guesses << " guesses";
        }
        return;
    }

    for (char c : charSet) {
        guesses++;
        attemptCombination(charSet, maxLength, passwordHash, currentString + c, found, guesses);
    }
}

void workerThread(const string &charSet, int maxLength, const string &passwordHash, atomic<bool> &found, atomic<long long> &guesses, int start, int stride) {
    for (int i = start; i < charSet.size() && !found; i += stride) {
        string currentString(1, charSet[i]);
        attemptCombination(charSet, maxLength, passwordHash, currentString, found, guesses);
    }
}

void incrementPasswordLength(const string &charSet, int &maxLength, const string &passwordHash, atomic<bool> &found, atomic<long long> &guesses, int numThreads) {
    cout << "Password not found in length " << maxLength << ", incrementing length\n";
    maxLength++;

    vector<thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(workerThread, ref(charSet), maxLength, ref(passwordHash), ref(found), ref(guesses), i, numThreads);
    }

    for (auto &t : threads) {
        t.join();
    }
}

int main(int argc, char* argv[]){
    string charSet = "123abcdefghijklmnopqrstuvwxyz456ABCDEFGHIJKLMNOPQRSTUVWXYZ7890!@#$%^&*()-_=+[]{}|,'.<>?~";
    string passwordHash;
    int maxLength = 0;
    int argIndex = 1;

    if (argc < 2 || argc > 5) {
        cout << "Usage: " << argv[0] << " [-h password_hash | -p password] [password_length] [-o]\n";
        return 1;
    }

    if (string(argv[argIndex]) == "-h") {
        passwordHash = argv[argIndex + 1];
        argIndex += 2;
    } else if (string(argv[argIndex]) == "-p") {
        passwordHash = sha256(argv[argIndex + 1]);
        argIndex += 2;
    }

    argIndex++;

    if (argIndex < argc && string(argv[argIndex]) == "-o") {
        debug = true;
        argIndex++; 
    }

    if (argIndex < argc) {
        maxLength = atoi(argv[argIndex]);
    }

    atomic<bool> found(false);
    atomic<long long> guesses(0);

    auto start = high_resolution_clock::now();

    int numThreads = thread::hardware_concurrency();
    vector<thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(workerThread, ref(charSet), maxLength, ref(passwordHash), ref(found), ref(guesses), i, numThreads);
    }

    for (auto &t : threads) {
        t.join();
    }

    auto end = high_resolution_clock::now();
    duration<double, std::milli> duration_ms = end - start;

    while (!found) {
        incrementPasswordLength(charSet, maxLength, passwordHash, found, guesses, numThreads);
    }

    cout << " in " << duration_ms.count() / 1000 << "s\n";

    return 0;
}
