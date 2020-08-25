#ifndef CONTROL_H
#define CONTROL_H

#include <string>
#include <map>
#include <memory>
#include <thread>

#include "PolyM/Queue.h" //PolyM

#include "opencv2/core/utility.hpp"

#include <inotify-cpp/NotifierBuilder.h>

#include "Robot.h"
#include "ProcessImages.h"

class Control {
private:
    bool m_failedToBuild = true;            //Check if start is OK 
    bool isRunning = false;                 //Start loop
    std::string m_capturePath;              //Path to captured frames
    
    inotify::NotifierBuilder m_notifier;    //Inotify object for callback handle
    std::unique_ptr<ProcessImages> proc;    //OpenCV methods for image processing
    
    //threads
    //std::thread imageThread;
    std::thread processThread;
    std::thread m_monitorThread;

    //thread methods
    void getImage(const std::string& path);

    bool startInotify();                    //configure inotify

    //Callback for inotify
    using callBack = std::function<void(void)>;
    callBack m_notifyCallBack;

public:

    std::map<id, std::unique_ptr<Robot>> allyRobots;
    std::map<id, std::unique_ptr<Robot>> enemyRobots;
    position ballPos;

    //Queues
    PolyM::Queue cameraQueue;
    PolyM::Queue processQueue;
    PolyM::Queue commandQueue;
    PolyM::Queue robotQueue;

    Control(const std::string& capturePath);
    ~Control();

    void addRobot(const id mid, const color mprimaryColor, const bool misAlly=false);
    void onNewFile(const callBack& t_callback);

};

#endif
