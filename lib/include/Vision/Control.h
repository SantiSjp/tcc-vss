#ifndef CONTROL_H
#define CONTROL_H

#include <string>
#include <map>
#include <memory>
#include <thread>

#include "PolyM/Queue.h" //PolyM

#include "opencv2/core/utility.hpp"

#include "Robot.h"

class Control {
private:
    bool isRunning = false; //Start loop
    int imageSeqNum = 0;

    //threads
    std::thread imageThread;

    //thread methods
    void getImage();

public:

    std::map<id, std::unique_ptr<Robot>> allyRobots;
    std::map<id, std::unique_ptr<Robot>> enemyRobots;
    position ballPos;

    //Queues
    PolyM::Queue cameraQueue;
    PolyM::Queue processQueue;
    PolyM::Queue commandQueue;
    PolyM::Queue robotQueue;

    Control(){};
    ~Control();

    int start();
    void addRobot(const id mid, const color mprimaryColor, const bool misAlly=false);

};

#endif
