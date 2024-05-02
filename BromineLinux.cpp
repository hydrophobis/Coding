#include <BromineMath.h>

int main(){

    Config config;
    config.clearTerminal = true;
    config.windows = false;

    Object3D obj;
    obj.ay = -9.81;
    obj.az = -0.1;
    obj.ax = 0.7;
    obj.velocity = {0.7, 0.0, 1.6};
    obj.energyRetention = 0.99;
    obj.position = {1.96, 23.9, 94.763423};

    while(true){
        obj.frame3D(1.0, config);
        obj.output3D();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}