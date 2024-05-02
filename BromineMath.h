#ifndef BROMINE_MATH_H
#define BROMINE_MATH_H

#include <iostream> // For standard I/O
#include <vector> // For vector operations
#include <cmath> // For math functions
#include <thread> // For threading and delays

#define RESET   "\033[0m"   // Resets color to default
#define RED     "\033[31m"  // Red text
#define GREEN   "\033[32m"  // Green text
#define YELLOW  "\033[33m"  // Yellow text
#define BLUE    "\033[34m"  // Blue text
#define MAGENTA "\033[35m"  // Magenta text
#define CYAN    "\033[36m"  // Cyan text
#define WHITE   "\033[37m"  // White text (default)
#define BOLONG DOUBLE    "\033[1m"   // Bolong double text

class Config {
public:
    bool clearTerminal = false;
    bool windows = false;
    bool outputVectorResults = true;

    void config() {
        if (clearTerminal) {
            if (windows) {
                system("clr"); // Windows clear command
            } else {
                system("clear"); // Linux clear command
            }
        }
    }
};

class Vector2D {
public:
    std::vector<long double> p1;
    std::vector<long double> p2;

    long double distance2D(long double x1, long double y1, long double x2, long double y2) {
        long double dx = x2 - x1;
        long double dy = y2 - y1;
        return std::sqrt(dx * dx + dy * dy);
    }

    long double distance2D(std::vector<long double> p1, std::vector<long double> p2) {
        long double dx = p2[0] - p1[0];
        long double dy = p2[1] - p1[1];
        return std::sqrt(dx * dx + dy * dy);
    }
};

class Vector3D : public Vector2D {
public:
    std::vector<long double> p1;
    std::vector<long double> p2;
    std::vector<long double> p3;
    std::vector<long double> p4;

    long double distance3D() {
        long double dx = p2[0] - p1[0];
        long double dy = p2[1] - p1[1];
        long double dz = p2[2] - p1[2];
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    long double planeArea3D() {
        long double a = distance2D(p2[0], p1[0], p2[1], p1[1]);
        long double b = distance2D(p3[0], p2[0], p3[1], p2[1]);
        long double c = distance2D(p3[0], p1[0], p3[1], p1[1]);

        long double s = (a + b + c) / 2;

        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }

    long double tetrahedronVolume() {
        const auto& a = p1;
        const auto& b = p2;
        const auto& c = p3;
        const auto& d = p4;

        std::vector<long double> ab = {b[0] - a[0], b[1] - a[1], b[2] - a[2]};
        std::vector<long double> ac = {c[0] - a[0], c[1] - a[1], c[2] - a[2]};
        std::vector<long double> ad = {d[0] - a[0], d[1] - a[1], d[2] - a[2]};

        long double determinant = (
            ab[0] * (ac[1] * ad[2] - ac[2] * ad[1]) +
            ab[1] * (ac[2] * ad[0] - ac[0] * ad[2]) +
            ab[2] * (ac[0] * ad[1] - ac[1] * ad[0])
        );

        return std::abs(determinant) / 6.0;
    }
};

class Object2D {
public:
    long double frame = 0.0;
    long double gravity;
    long double airResistance;
    std::vector<long double> velocity;
    std::vector<long double> position;

    void frame2D(long double step, bool output) {
        if (output) {
            // Handle output logic here
        }
        frame += step;
        velocity[1] += gravity * step;
        velocity[0] += airResistance * step;

        position[0] += velocity[0] * step;
        position[1] += velocity[1] * step;
    }

    void output2D() {
        std::cout << "Position: (" << MAGENTA << position[0] << ", " << position[1] << ")"
                  << "\nVelocity: (" << MAGENTA << velocity[0] << ", " << velocity[1] << ")"
                  << "\nGravity: " << MAGENTA << gravity << " | Air Resistance: "
                  << MAGENTA << (airResistance * -1.0) << "\n\n";
    }
};

class Object3D : public Object2D {
public:
    std::vector<long double> velocity;
    std::vector<long double> position;

    void frame3D(long double step) {
        frame += step;
        velocity[1] += gravity * step;
        velocity[0] += airResistance * step;
        velocity[2] += airResistance * step;

        position[0] += velocity[0] * step;
        position[1] += velocity[1] * step;
        position[2] += velocity[2] * step;
    }

    void output3D() {
        std::cout << "Frame: " << MAGENTA << frame 
                  << "\nPosition: (" << MAGENTA << position[0] << ", " << MAGENTA << position[1] << ", " << MAGENTA << position[2] << ")"
                  << "\nVelocity: (" << MAGENTA << velocity[0] << ", " << MAGENTA << velocity[1] << ", " << MAGENTA << velocity[2] << ")"
                  << "\nGravity: " << MAGENTA << gravity 
                  << " | Air Resistance: " << MAGENTA << (airResistance * -1.0) << "\n\n";
    }
};

#endif // VECTOR_AND_OBJECTS_HPP
