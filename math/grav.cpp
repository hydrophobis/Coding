#include <iostream>
#include <cmath>

// Function to calculate the gravitational force between two objects
double calculateGravitationalForce(double mass1, double mass2, double distance) {
    const double G = 6.67430e-11; // Gravitational constant (m^3 kg^-1 s^-2)
    double force = (G * mass1 * mass2) / pow(distance, 2);
    return force;
}

int main() {
    double mass1, mass2, distance;

    std::cout << "Enter the mass of the first object (kg): ";
    std::cin >> mass1;

    std::cout << "Enter the mass of the second object (kg): ";
    std::cin >> mass2;

    std::cout << "Enter the distance between the two objects (m): ";
    std::cin >> distance;

    // Calculate the gravitational force between the two objects
    double gravitationalForce = calculateGravitationalForce(mass1, mass2, distance);

    std::cout << "The gravitational force between the two objects is: " << gravitationalForce << " N" << std::endl;

    return 0;
}
