// 101 Thousand Guesses Per Second
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

// Function to generate combinations
void generateCombinations(const string& charSet, int maxLength, const string& password, int& guesses) {
    vector<char> currentCombination(maxLength);
    string currentString;

    auto start = chrono::steady_clock::now();

    while (currentString != password) {
        currentString = "";
        for (int i = 0; i < maxLength; ++i) {
            currentString += charSet[currentCombination[i]];
        }

        // Increment the combination
        int pos = maxLength - 1;
        while (pos >= 0 && ++currentCombination[pos] == charSet.size()) {
            currentCombination[pos--] = 0;
        }

        cout << currentString << "\n";

        ++guesses;

        // Check if current combination matches the password string
        if (currentString == password) {
            auto end = chrono::steady_clock::now();
            chrono::duration<double> elapsed_seconds = end - start;
            cout << "Found " << password << " with " << guesses << " guesses" << " in " << elapsed_seconds.count() << " seconds" << endl;
            return;
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

    int guesses = 0;
    generateCombinations(charSet, maxLength, password, guesses);

    return 0;
}
