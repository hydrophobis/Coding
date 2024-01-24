#include <iostream>
 
double pi(long int precision){
    double sum = 0.0;
    int sign = 1;
    for (int i = 0; i < precision; ++i) {           
        sum += sign/(2.0*i+1.0);
        sign *= -1;
    }
    std::cout << 4.0*sum;
}


int main(){
    pi(2000000000000);
}