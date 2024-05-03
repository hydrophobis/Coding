#include "BromineMath.h"

int main(){

    Config config;
    config.clearTerminal = true;
    config.windows = false;
/*
    Object3D obj;
    obj.velocity = {0.7, 0.0, 1.6};
    obj.position = {1.96, 23.9, 94.763423};
*/
    QuantumObject qobj;
    qobj.qrand = qobj.setQRand();
    qobj.position = {0.0, 0.0, 0.0};

    Electron e;
    e.linkedObject = qobj;
    e.INITIALIZE(e);
    e.move(e);
    e.outputElectron(e);

    return 0;

    while(true){
        //obj.frame3D(1.0);
        //obj.output3D();
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}