#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;
string password;

bool checkPw(string pw){
     if (pw == password){
          return true;
     } else {
          return false;
     }
}

int main(){
     string chars[52] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s",
                         "t", "u", "v", "w", "x", "y", "z", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L",
                         "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
     int charCount = 1;
     int pwLength;
     int charCounter = 1;
     string char2 = NULL;
     string char1 = NULL;
     string currentPw = "a";

     cout << "Password: ";
     cin >> password;
     cout << "\n";
     cout << "Password Length: ";
     cin >> pwLength;
     

     // While guessed password length is equal to or less than password, make a guess
     while (currentPw != password) {

          char1 = chars[rand() % 0 + 51];
          char2 = chars[rand() % 0 + 51];

          currentPw = currentPw + char2;

          if (currentPw.length() > pwLength){
               currentPw = "";
          }

          if (checkPw(currentPw) == true){
               cout << "Password Guessed: " << currentPw;
               return 0;
          }
     }
}