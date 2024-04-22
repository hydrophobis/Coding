// Escape Velocity From Any Celestial Body
#include <iostream>
#include <cmath>

// Gravitational constant (m^3 kg^-1 s^-2)
const double G = 6.674e-11;

// Function to calculate escape velocity
double escapeVelocity(double mass, double radius) {
    return sqrt(2 * G * mass / radius);
}

int main() {
    // Variables to store mass and radius
    double mass, radius;

    // Prompt user for mass and radius
    std::cout << "Enter the mass of the celestial body (in kg): ";
    std::cin >> mass;
    std::cout << "Enter the radius of the celestial body (in meters): ";
    std::cin >> radius;

    // Calculate escape velocity
    double ve = escapeVelocity(mass, radius);

    // Display the escape velocity
    std::cout << "The escape velocity is: " << ve << " m/s" << std::endl;

    return 0;
}
