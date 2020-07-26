#ifndef CONTROL_H
#define CONTROL_H

#include <string>
#include <map>

#include "Robot.h"

class Control {
private:
    bool isRunning = false;

    std::map<id, Robot> allyRobots;
    std::map<id, Robot> enemyRobots;
    position ballPos;

    //queues ...

public:
    Control(){};
    ~Control();

    int Start();
    bool addRobot();
    //command getCommand();

};

#endif
