// 7.4 Million Guesses Per Second
// Aid! 0.7720547s in 630967
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

void generateCombinations(const string& charSet, int maxLength, const string& password, string& currentString, int& guesses) {
    static auto start = chrono::steady_clock::now();

    if (currentString == password) {
        auto end = chrono::steady_clock::now();
        chrono::duration<double> elapsed_seconds = end - start;
        cout << "Found " << password << " with " << guesses << " guesses" << " in " << elapsed_seconds.count() << " seconds" << endl;
        exit(0);
    }

    for (char c : charSet) {
        // Ensure the first character is a capital letter
        if (currentString.empty() && !isupper(c)) {
            continue;
        }
        
        currentString.push_back(c);
        ++guesses;

        // Check if the length of the currentString is within the maxLength limit
        if (currentString.length() <= maxLength) {
            generateCombinations(charSet, maxLength, password, currentString, guesses);
        }

        // Remove the last character to try the next combination
        currentString.pop_back();
    }

    cout << currentString << "\n";
}

void generateCombinations2(const string& charSet, int maxLength, const string& password, string& currentString, int& guesses) {
    static auto start = chrono::steady_clock::now();

    if (currentString == password) {
        auto end = chrono::steady_clock::now();
        chrono::duration<double> elapsed_seconds = end - start;
        cout << "Found " << password << " with " << guesses << " guesses" << " in " << elapsed_seconds.count() << " seconds" << endl;
        exit(0);
    }

    for (char c : charSet) {
        // Ensure the first character is a capital letter
        if (currentString.empty() && !isupper(c)) {
            continue;
        }

        if (currentString.length() != 1 && !islower(c)){
            continue;
        }
        
        currentString.push_back(c);
        ++guesses;

        // Check if the length of the currentString is within the maxLength limit
        if (currentString.length() <= maxLength) {
            generateCombinations(charSet, maxLength, password, currentString, guesses);
        }

        // Remove the last character to try the next combination
        currentString.pop_back();
    }

    cout << currentString << "\n";
}

int main() {
    string charSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+[]{}|,',.<>?~";

    string password;
    int maxLength;
    bool algorithim;

    cout << "Password: ";
    cin >> password;

    cout << "Length: ";
    cin >> maxLength;

    cout << "0 for Aa11! format, 1 for Aa1!1";
    cin >> algorithim;

    string currentString;
    int guesses = 0;

    generateCombinations(charSet, maxLength, password, currentString, guesses);

    return 0;
}
