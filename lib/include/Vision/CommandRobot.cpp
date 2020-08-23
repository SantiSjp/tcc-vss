#include "CommandRobot.h"
#include "PolyM/Msg.h"
#include "PolyM/Queue.h"
#include <string>

void CommandRobot::insertCommandInQueue(int idRobot, std::vector<double> pos){
    std::map<int,std::vector<double>> msg;
    msg[idRobot] = pos;
    
    control.commandQueue.put(PolyM::DataMsg<std::map<int,std::vector<double>>>(0,msg));
}