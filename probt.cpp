#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Function to attempt to find the seed of an LCG
// given the first output and the LCG parameters
bool findSeed(int firstOutput, int a, int c, int m, int& seed) {
    for (int possibleSeed = 0; possibleSeed < m; ++possibleSeed) {
        if ((a * possibleSeed + c) % m == firstOutput) {
            seed = possibleSeed;
            return true; // Seed found
        }
    }
    return false; // Seed not found
}

int main() {

    srand(time(0));
    int output = rand()

    // LCG parameters
    int a = 2; // example multiplier
    int c = 3; // example increment
    int m = 6; // example modulus (2^32)

    int firstOutput = output; // Example output

    // Seed to be found
    int seed;
    if (findSeed(firstOutput, a, c, m, seed)) {
        std::cout << "Seed found: " << seed << std::endl;
    } else {
        std::cout << "Seed not found." << std::endl;
    }

    return 0;
}
