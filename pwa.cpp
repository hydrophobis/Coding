#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

void generateCombinations(const string& charSet, int maxLength, const string& password, string& currentString, int& guesses, int lowercaseCount, int numSymbolCount) {
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

        // Ensuring the first character is a capital letter
        if (currentString.length() == 1 && !isupper(c)) {
            currentString.pop_back();
            continue;
        }

        // Checking if the current character is a lowercase letter or a number/symbol
        if (islower(c)) {
            lowercaseCount++;
        } else if (!isalpha(c)) {
            numSymbolCount++;
        }

        // Checking if the length of the currentString is within the maxLength limit
        if (currentString.length() <= maxLength) {
            // Generating combinations with different ratios of lowercase letters to numbers/symbols
            if (lowercaseCount < currentString.length() / 2 && numSymbolCount < currentString.length() / 2) {
                generateCombinations(charSet, maxLength, password, currentString, guesses, lowercaseCount, numSymbolCount);
            }
        }

        cout << currentString << "\n";

        // Undoing the changes for the next iteration
        currentString.pop_back();
        if (islower(c)) {
            lowercaseCount--;
        } else if (!isalpha(c)) {
            numSymbolCount--;
        }
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
    int lowercaseCount = 0;
    int numSymbolCount = 0;

    generateCombinations(charSet, maxLength, password, currentString, guesses, lowercaseCount, numSymbolCount);

    return 0;
}
