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
    // Using block
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    // Start time
    auto t1 = high_resolution_clock::now();

    // Run function
    fib(12);

    // End time
    auto t2 = high_resolution_clock::now();

    // Getting number of milliseconds as a double.
    duration<double, std::milli> timeDouble = t2 - t1;

    // Calculating Score
    std::cout << 100000 - (timeDouble.count() * 1000000) << " FTTT score\n";

    // Return success
    return 0;

}