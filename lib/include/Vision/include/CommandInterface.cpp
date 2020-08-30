#include <string>
#include <utility>

#include "CommandInterface.h"

CommandInterface::CommandInterface(const std::string& path) : control(path){}


void CommandInterface::addNewRobot(const int id, const vss::color& newColor, const bool ally) {
    control.addRobot(id, newColor, ally);
}

void CommandInterface::insertCommandInQueue(int idRobot, const int posX, const int posY){
    control.putCommand(CommandFactory::makeNew(idRobot, posX, posY));
}