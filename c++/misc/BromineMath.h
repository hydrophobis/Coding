#ifndef BROMINE_MATH_H
#define BROMINE_MATH_H

#include <iostream> // For standard I/O
#include <vector> // For vector operations
#include <cmath> // For math functions
#include <thread> // For threading and delays
#include <algorithm>
#include <ctime>
#include <random>
#include <limits>

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
        std::cout << "Position: ("  << position[0] << ", " << position[1] << ")"
                  << "\nVelocity: ("  << velocity[0] << ", " << velocity[1] << ")"
                  << "\nGravity: "  << gravity << " | Air Resistance: "
                   << (airResistance * -1.0) << "\n\n";
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
        std::cout << "Frame: "  << frame 
                  << "\nPosition: ("  << position[0] << ", "  << position[1] << ", "  << position[2] << ")"
                  << "\nVelocity: ("  << velocity[0] << ", "  << velocity[1] << ", "  << velocity[2] << ")"
                  << "\nGravity: "  << gravity 
                  << " | Air Resistance: "  << airResistance << "\n\n";
    }
};

class QuantumObject : public Object3D {
public:
    bool entanglementSpin;

    struct QuantumObjectList {
        std::vector<int> ids;
        std::vector<bool> spins;
    };

    QuantumObjectList quantumObjList;
    int particleId;
    int entanglementId; 
    long int qrand;
    bool spin;


    long long int setQRand() {
        std::random_device rd;
        long long int qrand = rd();

        if (qrand == 0) {
            qrand = time(NULL);
        }

        // Seed with the obtained value
        srand(qrand);

        std::cout << "Seed for qrand: " << qrand << std::endl;

        // Return the seed value for later use or verification
        return qrand;
    }

    void updateState(QuantumObjectList list, bool spin, bool entanglementSpin, int entanglementId){
        entanglementSpin = list.spins[entanglementId];
        spin = !entanglementSpin;
    }

    void frame3DQ(long double step) {
        frame += step;
        velocity[1] += gravity * step;
        velocity[0] += airResistance * step;
        velocity[2] += airResistance * step;

        position[0] += velocity[0] * step;
        position[1] += velocity[1] * step;
        position[2] += velocity[2] * step;
    }

    void output3DQ() {
        std::cout << "Frame: "  << frame 
                  << "\nPosition: ("  << position[0] << ", "  << position[1] << ", "  << position[2] << ")"
                  << "\nVelocity: ("  << velocity[0] << ", "  << velocity[1] << ", "  << velocity[2] << ")"
                  << "\nGravity: "  << gravity 
                  << " | Air Resistance: "  << airResistance << " | Spin: " << spin << " | Entanglement ID: " << entanglementId << " | QRand: " << qrand << "\n\n";
    }
};

class Proton : public QuantumObject {
    int charge = 1;
};

class Electron : public QuantumObject {
public:
    QuantumObject linkedObject;
    long double distance; // Distance from linkedObject
    long double maxDistance = 0.0001; // Farthest possible from linkedObject
    long double factor = 0.0000000000000000000000000001;
    int charge = -1;

    void INITIALIZE(Electron& e){
        e.position.push_back(0.0);
        e.position.push_back(0.0);
        e.position.push_back(0.0);
    }

    void move(Electron& electron) {
        double randFactor = rand() % 1000 / 1000.0 * factor;  // Random factor scaling
        std::cout << electron.linkedObject.qrand << std::endl; 
        std::cout << randFactor << std::endl;

        // Calculate new potential positions
        electron.position[0] += randFactor * (rand());
        electron.position[1] += randFactor * (rand());
        electron.position[2] += randFactor * (rand());

        std::cout << electron.position[0] << std::endl;
        std::cout << electron.position[1] << std::endl;
        std::cout << electron.position[2] << std::endl;

        // Calculate the distance to the linkedObject
        double distance = sqrt(
            pow(electron.linkedObject.position[0] - electron.position[0], 2) +
            pow(electron.linkedObject.position[1] - electron.position[1], 2) +
            pow(electron.linkedObject.position[2] - electron.position[2], 2)
        );

        // If the new position is too far, revert to previous or scale back
        if (distance > electron.maxDistance) {
            // Move only to the maximum allowable distance
            double scaleBackFactor = electron.maxDistance / distance;
            electron.position[0] = linkedObject.position[0] + (electron.position[0] - linkedObject.position[0]) * scaleBackFactor;
            electron.position[1] = linkedObject.position[1] + (electron.position[1] - linkedObject.position[1]) * scaleBackFactor;
            electron.position[2] = linkedObject.position[2] + (electron.position[2] - linkedObject.position[2]) * scaleBackFactor;
        }
    }

    void outputElectron(Electron& e){
        std::cout << "Position: (" << e.position[0] << ", " << e.position[1] << ", " << e.position[2] << ")" << std::endl;
        std::cout << "Distance from linked object: " << e.distance << std::endl;
    }
};



#endif // VECTOR_AND_OBJECTS_HPP
