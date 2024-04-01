#include <random>
#include <ctime>
#include <iostream>

using namespace std;

int main(){

    const int steps = 100;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distrib(0.0, 1.0);

    double px = distrib(gen);
    double py = distrib(gen);
    double pz = distrib(gen);

    cout << "Steps: " << steps << "\n";

    
}