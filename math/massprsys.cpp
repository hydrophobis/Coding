// Mass-Spring System
#include <iostream>
#include <cmath>

// Function to calculate the position of the mass at time t
double calculatePosition(double amplitude, double frequency, double phaseShift, double dampingCoefficient, double mass, double springConstant, double time) {
    double angularFrequency = 2 * M_PI * frequency;
    double exponent = -dampingCoefficient * time / (2 * mass);
    double position = amplitude * exp(exponent) * cos(angularFrequency * time + phaseShift);
    return position;
}

int main() {
    double amplitude = 1.0;          // Amplitude of the oscillation (m)
    double frequency = 1.0;          // Frequency of the oscillation (Hz)
    double phaseShift = 0.0;         // Phase shift of the oscillation (radians)
    double dampingCoefficient = 0.1; // Damping coefficient (kg/s)
    double mass = 1.0;               // Mass attached to the spring (kg)
    double springConstant = 1.0;     // Spring constant (N/m)

    int numSteps = 100;              // Number of time steps
    double timeStep = 0.1;           // Time step size (s)

    std::cout << "Time (s)\tPosition (m)" << std::endl;
    for (int i = 0; i <= numSteps; ++i) {
        double time = i * timeStep;
        double position = calculatePosition(amplitude, frequency, phaseShift, dampingCoefficient, mass, springConstant, time);
        std::cout << time << "\t\t" << position << std::endl;
    }

    return 0;
}
