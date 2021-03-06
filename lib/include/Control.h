#ifndef CONTROL_H
#define CONTROL_H

#include <string>
#include <map>
#include <memory>

#include "PolyM/Queue.h" //PolyM
#include "Robot.h"

class Control {
private:
    bool isRunning = false; //Start loop

    std::map<id, std::unique_ptr<Robot>> allyRobots;
    std::map<id, std::unique_ptr<Robot>> enemyRobots;
    position ballPos;

    //Queues
    PolyM::Queue cameraQueue;
    PolyM::Queue processQueue;
    PolyM::Queue commandQueue;
    PolyM::Queue robotQueue;

    //command getCommand();

public:
    Control(){};
    ~Control();

    int Start();
    void addRobot(const id mid, const color mprimaryColor, const bool misAlly=false);

};

#endif
