#ifndef CONTROL_H
#define CONTROL_H

#include <string>
#include <map>
#include <memory>
#include <thread>

#include "PolyM/Queue.h" //PolyM

#include "opencv2/core/utility.hpp"

#include <inotify-cpp/NotifierBuilder.h>

#include "DataTypes.h"
#include "Robot.h"
#include "ProcessImages.h"
#include "Command.h"

namespace vss {
class Control {
private:
    bool m_failedToBuild = true;            //Check if start is OK 
    bool isRunning = false;                 //Start loop
    std::string m_capturePath;              //Path to captured frames
    
    //Position maps
    std::map<id, std::unique_ptr<Robot>> allyRobots;
    std::map<id, std::unique_ptr<Robot>> enemyRobots;
    position ballPos;

    //Queues
    PolyM::Queue cameraQueue;
    PolyM::Queue processQueue;
    PolyM::Queue commandQueue;
    PolyM::Queue robotQueue;

    inotify::NotifierBuilder m_notifier;    //Inotify object for callback handle
    std::unique_ptr<ProcessImages> proc;    //OpenCV methods for image processing
    
    //threads
    std::thread processThread;
    std::thread m_monitorThread;

    //thread methods
    void getImage(const std::string& path);
    bool startInotify();                    //configure inotify

public:
    Control(const std::string& capturePath);
    ~Control();

    void addRobot(const id mid, const color mprimaryColor, const bool misAlly=false);
    
    void putCommand(Command cmd);
    position getAllyPos (const id allyID);

};

}

#endif
