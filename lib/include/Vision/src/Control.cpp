#include <ctime>
#include <iostream>
#include <chrono>
#include <experimental/filesystem>

#include "Control.h"

#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <spdlog/sinks/basic_file_sink.h>

namespace vss {
Control::Control(const std::string& capturePath) 
    : m_capturePath(capturePath) {
        
    m_logger = std::make_unique<Logger>("Control", "logs/vision_log.txt", Level::debug);
    m_logger->info("System Start");

    startInotify();

    m_processImages = std::make_unique<ProcessImages>(cameraQueue, processQueue);
    m_processPosition = std::make_unique<ProcessPosition>(processQueue);

    //spawn threads
    m_processThread = std::thread(&ProcessImages::start, m_processImages.get());
    m_calculateThread = std::thread(&ProcessPosition::start, m_processPosition.get());
    
    m_isRunning = true;
}


bool Control::startInotify() {
    boost::filesystem::path path(m_capturePath);
    auto handleNotification = [&](inotify::Notification notification) {
        putInCameraQueue(notification.path.c_str());
    };

    auto events = {inotify::Event::create, inotify::Event::moved_to, inotify::Event::move};

    m_logger->info("Inotify watching path: %s",path);
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
        m_allyRobots[t_id] = std::make_unique<Robot>(t_id, cv::Mat(), t_isAlly);
        m_logger->debug(Logger::format("New robot [%d] added to allyRobots map.", t_id));

    } else {
        m_enemyRobots[t_id] = std::make_unique<Robot>(t_id, cv::Mat(), t_isAlly);
        m_logger->debug("New robot [%d] added to enemyRobots map.", t_id);
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
    std::experimental::filesystem::remove(path);
    m_logger->debug("Captured frame '%s'", path);
}


void Control::putInCommandQueue(const Command command) {
    m_logger->debug("Captured command RobotId: %d.", command.robotId);
    m_processPosition->calculateFuturePosition(command);
    commandQueue.put(PolyM::DataMsg<Command>(0, command));
}


position Control::getAllyPos(const id allyID){
    return m_processPosition->getCurrentField().getAlly(allyID).getPosition();
}


Control::~Control(){
    m_logger->info("System Stop");
    m_isRunning = false;
    m_notifier.stop();

    m_processImages->stop();
    m_processPosition->stop();

    m_calculateThread.join();
    m_processThread.join();
    m_monitorThread.join();
}

}