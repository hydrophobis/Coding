#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define stracc(s) s.internal
struct string {
    char *internal;

    int contains(char ch) {
        for (int i = 0; i < strlen(internal); i++) {
            if (internal[i] == ch) {
                return 1;
            }
        }
        return 0;
    }

    int contains(char *ch) {
        for (int i = 0; i < strlen(internal); i++) {
            for (int j = 0; j < strlen(ch); j++) {
                if (internal[i] == ch[j]) {
                    return 1;
                }
            }
        }
        return 0;
    }
};
typedef struct string string;
string string_init(char* str) {
    string out;
    out.internal = (char*)malloc(strlen(str) * sizeof(char));
    out.internal = str;
    return out;
}
bool findstr(char* str, char* search){
    int counter = 0;
    for(int i = 0; i < str.length(); i++;){
        if(str[i] == search[counter]){
            counter += 1;
        } else {
            counter = 0;
        }
        
        if(i == str.length() - 1){
            return false;
        }
    }
    return true;
}
    
