#include <iostream>
#include <vector>
#include <cmath>

// Check if a number is a perfect square
bool isPerfectSquare(int n) {
    int squareRoot = sqrt(n);
    return squareRoot * squareRoot == n;
}

// Function to check if a number can be made with 4 perfect squares
bool find(int n) {
    // We only need to search up to the square root of n
    int limit = sqrt(n);

    for (int a = 0; a <= limit; ++a) {
        for (int b = 0; b <= limit; ++b) {
            for (int c = 0; c <= limit; ++c) {
                int d = sqrt(n - a * a - b * b - c * c);
                // Check if d is a perfect square and the sum of squares equals n
                if (d * d + a * a + b * b + c * c == n && isPerfectSquare(d * d)) {
                    return true;
                }
            }
        }
    }
    return false;
}

// Function to check if all numbers up to a given number can be expressed as the sum of four perfect squares
bool check(int target) {
    for (int i = 1; i <= target; ++i) {
        if (!find(i)) {
            return false;  // If any number cannot be expressed, return false
        }
    }
    return true;  // If all numbers can be expressed, return true
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <target>" << std::endl;
        return 1;
    }

    int target = std::stoi(argv[1]);

    if (check(target)) {
        std::cout << "All numbers up to " << target << " can be made with 4 perfect squares." << std::endl;
    } else {
        std::cout << "Not all numbers up to " << target << " can be made as the sum of four perfect squares." << std::endl;
    }

    return 0;
}
