// Test file
// aid! 0.115973 in 5.7 Million
// 49.4 Million Guesses Per Second
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

// Function to generate combinations using recursive backtracking
void generateCombinations(const string& charSet, int maxLength, const string& password, string currentString, int& guesses) {
    static auto start = steady_clock::now();

    if (currentString == password) {
        auto end = steady_clock::now();
        double elapsed_seconds = duration<double>(end - start).count();
        cout << "Found " << password << " with " << guesses << " guesses" << " in " << elapsed_seconds << " seconds" << endl;
        exit(0);
    }

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
    string charSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+[]{}|,',.<>?~";

    string password;
    int maxLength;

    cout << "Password: ";
    cin >> password;

    cout << "Length: ";
    cin >> maxLength;

    string currentString;
    int guesses = 0;

    generateCombinations(charSet, maxLength, password, currentString, guesses);

    return 0;
}
