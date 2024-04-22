// 13500 Guesses Per Second
// aid! 604.345926 in 10.3 Million
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

string password;
int guessesI = 0;

bool checkPw(const string& pw) {
    cout << "Guessing password: " << pw << "               Guess number: " << guessesI << "\n";
    return pw == password;
}

int main() {
    string chars[100] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s",
                         "t", "u", "v", "w", "x", "y", "z", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L",
                         "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "1", "2", "3", "4", "5",
                         "6", "7", "8", "9", "0", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "-", "_", "=", "+",
                         "[", "]", "{", "}", "|", ",", "'", "<", ">", "/", "?", "~"};
    int pwLength = 0;

    cout << "Password: ";
    cin >> password;
    cout << "\n";
    cout << "Password Length: ";
    cin >> pwLength;

    auto start = high_resolution_clock::now();

    // While guessed password is not equal to password, make a guess
    while (true) {
        string currentPw = "";
        for (int j = 0; j < pwLength; ++j) {
            currentPw += chars[rand() % 89];
        };

        ++guessesI;

        if (checkPw(currentPw)) {
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - start);
            cout << "Guess successful: " << password << " in " << guessesI << " guesses, took " << duration.count()/1000 << "s\n";
            return 0;
        };
    }
}
