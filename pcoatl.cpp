// 490 Thousand Guessues Per Second
// aid! 0.2445s in 124 Thousand
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
        if (found) break;
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

int main() {
    string charSet = "123abcdefghijklmnopqrstuvwxyz456ABCDEFGHIJKLMNOPQRSTUVWXYZ7890!@#$%^&*()-_=+[]{}|,'.<>?~";
    string passwordHash;
    int maxLength;

    cout << "Hashed Input Password: ";
    cin >> passwordHash;
    cout << "Length Before Hashing: ";
    cin >> maxLength;

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

    if (!found) {
        cout << "Password not found within the specified length.\n";
    }
    cout << " in " << duration_ms.count() / 1000 << "s\n";

    return 0;
}
