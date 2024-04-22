// Pendulum Oscillation
#include <iostream>
#include <cmath>

// Function to calculate the period of oscillation for a simple pendulum
double calculatePeriod(double length, double gravity) {
    return 2 * M_PI * sqrt(length / gravity);
}

int main() {
    const double g = 9.81; // Acceleration due to gravity (m/s^2)
    double pendulumLength;

    std::cout << "Enter the length of the pendulum (m): ";
    std::cin >> pendulumLength;

    // Calculate the period of oscillation for the simple pendulum
    double period = calculatePeriod(pendulumLength, g);

    std::cout << "The period of oscillation for the simple pendulum is: " << period << " s" << std::endl;

    return 0;
}
