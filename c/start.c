#include <stdio.h>
#include <stdlib.h>

double math(char op, double num1, double num2){
    if(op == "+"){
        return num1 + num2;
    } else if(op == "-"){
        return num1 - num2;
    } else if(op == "*"){
        return num1 * num2;
    } else if(op == "/"){
        return num1 / num2;
    }
}

int main(int argc, char* argv[]){

    int i = 0;
    int counter = 0;

    double num1 = atoi(argv[1]);
    double num2 = atoi(argv[3]);
    char* operators[4] = {"+", "-", "*", "/"};
    char* multiplication[10];

    for(i < argc; i++){
        if(argv[i] == "*"){
            multiplication[counter] = multiplication + argv[i];
        }
        counter += 1;
    };
}
