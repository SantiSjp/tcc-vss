#include "Control.h"
#include <iostream>
#include <memory>

int Control::start(){
    //spawn threads
    
    isRunning = true;
    while(isRunning){
        std::cout << "running" << std::endl;

    }

    return 0;
}

void Control::addRobot(const id mid, 
    const color mprimaryColor, 
    const bool misAlly) {
    
    if(misAlly) {
        allyRobots[mid] = std::make_unique<Robot>(mid, mprimaryColor, misAlly);
        
    } else {
        enemyRobots[mid] = std::make_unique<Robot>(mid, mprimaryColor, misAlly);
    }
}

Control::~Control(){
    isRunning = false;
}