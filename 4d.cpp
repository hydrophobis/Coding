#include <iostream>
#include <vector>
#include <cmath>
#include <gmp.h>
#include <thread>

#define ld long double
#define RESET   "\033[0m"   // Resets color to default
#define RED     "\033[31m"  // Red text
#define GREEN   "\033[32m"  // Green text
#define YELLOW  "\033[33m"  // Yellow text
#define BLUE    "\033[34m"  // Blue text
#define MAGENTA "\033[35m"  // Magenta text
#define CYAN    "\033[36m"  // Cyan text
#define WHITE   "\033[37m"  // White text (default)
#define BOLD    "\033[1m"   // Bold text

using namespace std;

long double pow(long double num, int power){
    for(int i = 0; i < power; i++){
        num = num * num;
    }
    return num;
}

class Config {
public:
    bool clearTerminal = false;
    bool windows = false;
    bool outputVectorResults = true;
    

    void config(){
        if (clearTerminal == true){
            if (windows == false){
                system("clear");
            } else {
                system("clr");
            }
        }
    }
};

class Vector2D {
public:
    vector<ld> p1;
    vector<ld> p2;

    ld distance2D(ld x1, ld y1, ld x2, ld y2) {
        ld dx = x2 - x1;
        ld dy = y2 - y1;
        return std::sqrt(dx * dx + dy * dy);
    }

    ld distance2D(vector<ld> p1, vector<ld> p2){
        ld dx = p2[0] - p1[0];
        ld dy = p2[1] - p1[1];
        return std::sqrt(dx * dx + dy * dy);
    }

   /* ld area(){

    } */
};

// p1, p2 and p3 are verteces that need three values
class Vector3D : Vector2D {
public:
    vector<ld> p1;
    vector<ld> p2;
    vector<ld> p3;
    vector<ld> p4;

    // Finds the distance between two 3D points, takes no input(uses objects defined verteces) or input all three verteces
    ld distance3D(){
        
         
        ld dx = p2[0] - p1[0];
        ld dy = p2[1] - p1[1];
        ld dz = p2[2] - p1[2];
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    // Finds the distance between two 3D points, takes no input(uses objects defined verteces) or input all three verteces
    ld distance3D(vector<ld> p1, vector<ld> p2){
        ld dx = p2[0] - p1[0];
        ld dy = p2[1] - p1[1];
        ld dz = p2[2] - p1[2];
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    // finds the area of a 3d plane
    ld planeArea3D(){
        // p1 and p2 Plane
        ld a = distance2D(p2[0], p1[0], p2[1], p1[1]);
        // p2 and p3 Plane
        ld b = distance2D(p3[0], p2[0], p3[1], p2[1]);
        // p1 and p3 Plane
        ld c = distance2D(p3[0], p1[0], p3[1], p1[1]);

        ld s = (a + b +c)/2;

        return sqrt(s * (s - a) * (s - b) * (s-c));
    }

    // Function to calculate the volume of a tetrahedron given four 3D points
    ld tetrahedronVolume() {

        const auto& a = p1;
        const auto& b = p2;
        const auto& c = p3;
        const auto& d = p4;

        // Calculate the differences to create the vectors for the matrix
        std::vector<ld> ab = {b[0] - a[0], b[1] - a[1], b[2] - a[2]};
        std::vector<ld> ac = {c[0] - a[0], c[1] - a[1], c[2] - a[2]};
        std::vector<ld> ad = {d[0] - a[0], d[1] - a[1], d[2] - a[2]};

        // Calculate the determinant of the 3x3 matrix
        ld determinant = (
            ab[0] * (ac[1] * ad[2] - ac[2] * ad[1]) +
            ab[1] * (ac[2] * ad[0] - ac[0] * ad[2]) +
            ab[2] * (ac[0] * ad[1] - ac[1] * ad[0])
        );

        // The volume is 1/6 of the absolute value of the determinant
        return std::abs(determinant) / 6.0;
    }
};

// Requires four vertices with 4 values
class Vector4D : Vector3D {
    vector<ld> p1;
    vector<ld> p2;
    vector<ld> p3;
    vector<ld> p4;
    vector<ld> p5;
    ld time;
    

};

class Object2D {
public:
    long double frame = 0.0;
    ld gravity; // Acceleration due to gravity
    ld airResistance; // Acceleration due to air resistance
    vector<ld> velocity; // Velocity vector (2D: [vx, vy])
    vector<ld> position; // Position vector (2D: [x, y])

    // Updates velocity and position in 2D space
    void frame2D(ld step, bool output) {
        if(output == true){
            
        }
        frame += step;
        // Update vertical velocity due to gravity
        velocity[1] += gravity * step;
        velocity[0] += airResistance * step;

        // Update position based on velocity
        position[0] += velocity[0] * step; // Update x-position
        position[1] += velocity[1] * step; // Update y-position
    }

    void output2D(){
        cout << "Position: (" << MAGENTA << position[0] << ", " << position[1] << RESET <<")\n";
        cout << "Velocity: (" << MAGENTA << velocity[0] << ", " << velocity[1] << RESET << ")\n";
        cout << "Gravity: " << MAGENTA << gravity << " | Air Resistance: " << MAGENTA << (airResistance * -1.0) << RESET << "\n\n";
    }
};

class Object3D : Object2D {
public:
    ld gravity; // Acceleration due to gravity
    ld airResistance; // Acceleration due to air resistance
    vector<ld> velocity; // Velocity vector (3D: [vx, vy, vz])
    vector<ld> position; // Position vector (3D: [x, y, z])

    // Updates velocity and position in 3D space
    void frame3D(ld step, bool output) {
        frame += step;
        // Update vertical velocity due to gravity
        velocity[1] += gravity * step; // Assuming gravity is constant
        velocity[0] += airResistance * step;
        velocity[2] += airResistance * step;

        // Update position based on velocity
        position[0] += velocity[0] * step; // Update x-position
        position[1] += velocity[1] * step; // Update y-position
        position[2] += velocity[2] * step; // Update z-position
    }

    void output3D(){
        cout << "Frame: " << MAGENTA << BOLD << frame << RESET << "\n";
        cout << "Position: (" << MAGENTA << position[0] << RESET << ", " << MAGENTA << position[1] << RESET << ", " << MAGENTA << position[2] << RESET << ")\n";
        cout << "Velocity: (" << MAGENTA << velocity[0] << RESET << ", " << MAGENTA << velocity[1] << RESET << ", " << MAGENTA << velocity[2] << RESET << ")\n";
        cout << "Gravity: " MAGENTA << gravity << RESET << " | Air Resistance: " << MAGENTA << (airResistance * -1.0) << RESET << "\n\n";
    }
};

int main(){

    Config config;
    config.clearTerminal = true;
    config.windows = false;

    Object3D obj;

    obj.gravity = 9.81;
    obj.airResistance = 0.1;
    obj.position = {0.0, 10.0, 0.0};
    obj.velocity = {10.0, 1.0, 0.0};

    Vector2D vec;

    vec.p1 = {1.0, 4.0};
    vec.p2 = {5.9, 0.29317};

    Vector3D vec3;

    vec3.p1 = {1.0, 4.0, 3.0};
    vec3.p2 = {5.9, 0.29317, 8.926};

    while(false){
        obj.output3D();
        obj.frame3D(0.5);
        config.config();
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Sleep for 2 seconds
    }
}