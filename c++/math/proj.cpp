// Projectile Motion
#include <iostream>
#include <cmath>

// Function to calculate the horizontal distance traveled by the projectile
double horizontalDistance(double initialVelocity, double launchAngle, double time) {
    return initialVelocity * cos(launchAngle) * time;
}

// Function to calculate the vertical distance traveled by the projectile
double verticalDistance(double initialVelocity, double launchAngle, double time, double gravity) {
    return initialVelocity * sin(launchAngle) * time - 0.5 * gravity * time * time;
}

int main() {
    const double g = 9.81; // Acceleration due to gravity (m/s^2)
    double initialVelocity, launchAngle;
    
    std::cout << "Enter the initial velocity (m/s): ";
    std::cin >> initialVelocity;
    
    std::cout << "Enter the launch angle (degrees): ";
    std::cin >> launchAngle;
    launchAngle = launchAngle * M_PI / 180; // Convert degrees to radians

    // Calculate and display the trajectory for each second up to when the projectile hits the ground
    double time = 0;
    while (true) {
        double horizontalDist = horizontalDistance(initialVelocity, launchAngle, time);
        double verticalDist = verticalDistance(initialVelocity, launchAngle, time, g);

        if (verticalDist < 0) {
            // Projectile hits the ground, end loop
            break;
        }

        std::cout << "Time: " << time << "s, Horizontal Distance: " << horizontalDist << "m, Vertical Distance: " << verticalDist << "m\n";
        
        // Increment time
        time += 0.1; // You can adjust the time step for finer or coarser resolution
    }

    return 0;
}
