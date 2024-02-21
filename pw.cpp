#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

string password;
bool outputAll = true;
int guesses = 0;

bool checkPw(string pw){
    if(outputAll == true){
        cout << "Guessing password: " << pw << "               Guess number: " << guesses << "\n";
    }
    if (pw == password){
        return true;
    } else {
        return false;
    }
}

int main(){
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

    int i = 0;

    // While guessed password is not equal to password, make a guess
    while (true) {
        string currentPw = "";
        for (int j = 0; j < pwLength; ++j) {
            currentPw += chars[rand() % 85];
        }

        guesses++;

        if (checkPw(currentPw) == true) {
            cout << "Guess successful: " << password << " in " << guesses << " guesses" << "\n";
            return 0;
        }
    }
}