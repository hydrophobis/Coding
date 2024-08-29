#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>

using namespace std;
using namespace chrono;

atomic<bool> found(false);
mutex mtx;

void generateCombinations(const string &charSet, int maxLength, const string &password, string currentString, atomic<int> &guesses) {
    if (found) return;

    if (currentString.length() == maxLength) return;

    for (char c : charSet) {
        if (found) return;

        currentString.push_back(c);
        guesses.fetch_add(1, std::memory_order_relaxed);

        if (currentString == password) {
            lock_guard<mutex> lg(mtx);
            if (!found) {  // Double-check in case another thread found it first
                found = true;
                cout << "Found " << password << " with " << guesses.load() << " guesses" << endl;
            }
            return;
        }

        generateCombinations(charSet, maxLength, password, currentString, guesses);
        currentString.pop_back();
    }
}

int main() {
    string charSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+[]{}|,'.<>?~";
    string password;
    int maxLength;

    cout << "Password: ";
    cin >> password;

    cout << "Length: ";
    cin >> maxLength;

    auto start = high_resolution_clock::now();

    vector<thread> threads;
    int numThreads = thread::hardware_concurrency();
    vector<atomic<int>> threadGuesses(numThreads);

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&, i]() {
            generateCombinations(charSet, maxLength, password, string(), threadGuesses[i]);
        });
    }

    for (auto &t : threads) {
        t.join();
    }

    int totalGuesses = 0;
    for (const auto &tg : threadGuesses) {
        totalGuesses += tg.load();
    }

    auto end = high_resolution_clock::now();
    duration<double> elapsed_seconds = end - start;
    cout << "Time elapsed: " << elapsed_seconds.count() << "s\n";

    return 0;
}
