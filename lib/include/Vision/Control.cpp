#include "Control.h"
#include <iostream>
#include <memory>

int Control::start(){
    //spawn threads

    commandThread = std::thread(&Control::getCommand,this);

    isRunning = true;

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

void Control::getCommand(){
    while (isRunning){
        auto cmd = commandQueue.get(500);

        if(cmd->getMsgId() != PolyM::MSG_TIMEOUT){
            auto aux = dynamic_cast<PolyM::DataMsg<std::map<int,std::vector<double>>>&>(*cmd).getPayload();
            std::cout << aux[1][0] << std::endl;
            // Executar funcoes para validar comando para o robo.
        }
    }
}

Control::~Control(){
    isRunning = false;
    commandThread.join();
}