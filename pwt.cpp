// Test file, No output
// 49.2 Million Guesses Per Second
// aid! 0.116392s in 5.7 Million Guesses
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

string currentString;
int guesses = 0;

// Function to generate combinations using recursive backtracking
void generateCombinations(string charSet, int maxLength, string password) {
    static auto start = chrono::steady_clock::now();

    if (currentString == password) {
        auto end = chrono::steady_clock::now();
        chrono::duration<double> elapsed_seconds = end - start;
        cout << "Found " << password << " with " << guesses << " guesses" << " in " << elapsed_seconds.count() << " seconds" << endl;
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

    generateCombinations(charSet, maxLength, password);

    return 0;
}
