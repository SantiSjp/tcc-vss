#ifndef CONTROL_H
#define CONTROL_H

#include <string>
#include <map>
#include <memory>
#include <thread>

#include "PolyM/Queue.h" //PolyM
#include "opencv2/core/utility.hpp"
#include <inotify-cpp/NotifierBuilder.h>
#include <spdlog/spdlog.h>
#include <boost/format.hpp>

#include "DataTypes.h"
#include "Robot.h"
#include "ProcessImages.h"
#include "Command.h"

namespace vss {
class Control {
private:
    bool m_failedToBuild = true;            //Check if start is OK 
    bool m_isRunning = false;               //Start loop
    std::string m_capturePath;              //Path to captured frames
    
    std::shared_ptr<spdlog::logger> m_logger;

    //Position maps
    std::map<id, std::unique_ptr<Robot>> m_allyRobots;
    std::map<id, std::unique_ptr<Robot>> m_enemyRobots;
    position m_ballPos;

    //Queues
    PolyM::Queue cameraQueue;
    PolyM::Queue processQueue;
    PolyM::Queue commandQueue;
    PolyM::Queue robotQueue;

    inotify::NotifierBuilder m_notifier;                //Inotify object for callback handle
    std::unique_ptr<ProcessImages> m_processImages;     //OpenCV methods for image processing
    
    //threads
    std::thread m_processThread;
    std::thread m_monitorThread;

    //thread methods
    void putInCameraQueue(const std::string& path);
    bool startInotify();                                //configure inotify

    template<typename... Arguments>
    std::string format(std::string const& fmt, Arguments&&... args) {
        boost::format f(fmt);
        int unroll[] {0, (f % std::forward<Arguments>(args), 0)...};
        static_cast<void>(unroll);

        return boost::str(f);
    }

public:
    Control(const std::string& capturePath);
    ~Control();
    
    void addRobot(const id t_id, const color t_primaryColor, const bool t_isAlly=false);
    
    void putInCommandQueue(Command cmd);
    position getAllyPos (const id allyID);

};

}

#endif
