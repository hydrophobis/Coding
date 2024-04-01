#include <bits/stdc++.h> 
#include <chrono>
using namespace std; 


int fib(long int n) 
{ 
    if (n <= 1) 
        return n; 
    return fib(n - 1) + fib(n - 2); 
} 

int main(){
    
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    int in;
    int powerOf;

    std::cin >> in;

    auto t1 = high_resolution_clock::now();

    powerOf = pow(in, in) - in;

    std::cout << fib(in) + powerOf << "\n";

    auto t2 = high_resolution_clock::now();

    // Getting number of milliseconds as a double.
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_double.count() * 10000 << "ns\n";
    return 0;

}