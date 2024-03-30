// Test file
// aid! 0.184227 in 9.95 Million
// 56.9 Million Guesses Per Second
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>

using namespace std;
using namespace chrono;

atomic<bool> found(false);

void generateCombinations(const string &charSet, int maxLength, const string &password, string currentString, int &guesses) {
    if (found) return;

    if (currentString == password) {
        found = true;
        cout << "Found " << password << " with " << guesses << " guesses" << endl;
        return;
    }

    for (char c : charSet) {
        if (found) return;

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
    int guesses = 0;

    cout << "Password: ";
    cin >> password;

    cout << "Length: ";
    cin >> maxLength;

    auto start = high_resolution_clock::now();

    vector<thread> threads;
    int numThreads = thread::hardware_concurrency();

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(generateCombinations, cref(charSet), maxLength, cref(password), string(), ref(guesses));
    }

    for (auto &t : threads) {
        t.join();
    }

    auto end = high_resolution_clock::now();
    duration<double> elapsed_seconds = end - start;
    cout << "Time elapsed: " << elapsed_seconds.count() << "s\n";

    return 0;
}
