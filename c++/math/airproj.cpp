// Projectile Motion With Air Resistance
#include <iostream>
#include <cmath>

// Constants
const double g = 9.81;  // Acceleration due to gravity (m/s^2)
const double rho = 1.2;  // Air density (kg/m^3)
const double Cd = 0.5;   // Drag coefficient
const double A = 0.1;    // Cross-sectional area (m^2)
const double dt = 0.01;  // Time step (s)

// Function to calculate the magnitude of the velocity vector
double magnitude(double vx, double vy) {
    return sqrt(vx * vx + vy * vy);
}

// Function to calculate the drag force
double dragForce(double vx, double vy) {
    double v = magnitude(vx, vy);
    return 0.5 * rho * Cd * A * v * v;
}

int main() {
    // Initial conditions
    double angleDegrees, velocity;
    std::cout << "Enter launch angle (degrees): ";
    std::cin >> angleDegrees;
    std::cout << "Enter initial velocity (m/s): ";
    std::cin >> velocity;

    // Convert angle to radians
    double angleRadians = angleDegrees * M_PI / 180.0;

    // Initial velocity components
    double vx = velocity * cos(angleRadians);
    double vy = velocity * sin(angleRadians);

    // Initial position
    double x = 0.0;
    double y = 0.0;

    // Simulation loop
    while (y >= 0.0) {
        // Calculate drag force
        double Fx = -dragForce(vx, vy) * vx / magnitude(vx, vy);
        double Fy = -dragForce(vx, vy) * vy / magnitude(vx, vy) - g;

        // Update velocity
        vx += Fx / 1.0 * dt;
        vy += Fy / 1.0 * dt;

        // Update position
        x += vx * dt;
        y += vy * dt;

        // Output position
        std::cout << "Time: " << x / vx << " seconds, x: " << x << " meters, y: " << y << " meters\n";
    }

    return 0;
}
