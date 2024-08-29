// 6.3 Million Guesses Per Second
// Aid! 0.949004s in 5.7 Million
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

// Function to generate combinations using recursive backtracking
void generateCombinations(const string& charSet, int maxLength, const string& password, string& currentString, int& guesses) {
    static auto start = chrono::steady_clock::now();

    if (currentString == password) {
        auto end = chrono::steady_clock::now();
        chrono::duration<double> elapsed_seconds = end - start;
        cout << "Found " << password << " with " << guesses << " guesses" << " in " << elapsed_seconds.count() << " seconds" << endl;
        exit(0);
    }

    //cout << currentString << "\n";

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

    cout << "Password: ";
    cin >> password;

    cout << "Length: ";
    cin >> maxLength;

    string currentString;
    int guesses = 0;

    generateCombinations(charSet, maxLength, password, currentString, guesses);

    return 0;
}
