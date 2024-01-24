#include <iostream>
#include "pi.cpp"



int main(){
    int times;
    int i = 1;
    int num;
    std::cout << "Up to what number:";
    std::cin >> times;

    while(i <= times){
        num = i * i;
        std::cout << i << " " << num << "\n";
        i++;
    }

    pi(100);
}