#ifndef COMMANDROBOT_H
#define COMMANDROBOT_H

#include <map>
#include <memory>
#include <vector>

#include "Vision/Robot.h"
#include "Control.h"

class CommandRobot : public Control {
public:

    CommandRobot(const std::string& path);
    void insertCommandInQueue(int idRobot, std::vector<double> pos);

};


#endif // !COMMAND_H
