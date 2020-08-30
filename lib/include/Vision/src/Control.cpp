#include <ctime>
#include <iostream>
#include <chrono>
#include <experimental/filesystem>

#include "Control.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace vss {
Control::Control(const std::string& capturePath) 
    : m_capturePath(capturePath) {
    if(startInotify()) {
        m_failedToBuild = true;
    }

    m_processImages = std::make_unique<ProcessImages>(cameraQueue);

    //spawn threads
    m_processThread = std::thread(&ProcessImages::start, m_processImages.get());
    
    m_isRunning = true;
}


bool Control::startInotify() {
    boost::filesystem::path path(m_capturePath);
    auto handleNotification = [&](inotify::Notification notification) {
        //std::cout << notification.path.c_str() << std::endl;
        putInCameraQueue(notification.path.c_str());
    };

    auto events = {inotify::Event::create, inotify::Event::moved_to, inotify::Event::move};

    m_notifier = inotify::BuildNotifier();
    m_notifier.watchPathRecursively(path);
    m_notifier.onEvents(events, handleNotification);

    m_monitorThread = std::thread([&](){m_notifier.run();});
    return true;
}


void Control::addRobot(const id t_id, 
    const color t_primaryColor, 
    const bool t_isAlly) {
    if(t_isAlly) {
        m_allyRobots[t_id] = std::make_unique<Robot>(t_id, t_primaryColor, t_isAlly);
        
    } else {
        m_enemyRobots[t_id] = std::make_unique<Robot>(t_id, t_primaryColor, t_isAlly);
    }
    
}


void Control::putInCameraQueue(const std::string& path){
    cv::Mat image;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    image = cv::imread(path, cv::ImreadModes::IMREAD_COLOR);
    if(!image.data){
        return;
    }
    
    cameraQueue.put(PolyM::DataMsg<cv::Mat>(0,image));
}


void Control::putInCommandQueue(const Command command) {
    commandQueue.put(PolyM::DataMsg<Command>(0, command));
}


position Control::getAllyPos(const id allyID){
    return m_allyRobots[allyID]->getPosition();
}


Control::~Control(){
    m_isRunning = false;
    m_notifier.stop();

    m_processImages->stop();

    m_processThread.join();
    m_monitorThread.join();
}

}