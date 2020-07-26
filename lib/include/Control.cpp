#include "Control.h"
#include <iostream>

int Control::Start(){
    //spawn threads
    
    isRunning = true;
    while(isRunning){
        std::cout << "running" << std::endl;

    }

    return 0;
}

bool Control::addRobot() {
    
    return false;
}

Control::~Control(){
    isRunning = false;
}