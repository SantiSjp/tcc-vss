#ifndef COMMANDINTERFACE_H
#define COMMANDINTERFACE_H

#include <map>
#include <memory>
#include <vector>

#include "Vision/src/Robot.h"
#include "Vision/src/Control.h"
#include "Vision/src/DataTypes.h"

class CommandInterface {
private:
    vss::Control control;

public:
    CommandInterface(const std::string& path);

    void addNewRobot(const int id, const vss::color& newColor, const bool ally);
    void insertCommandInQueue(int idRobot, const int posX, const int posY);

};


#endif // !COMMAND_H
