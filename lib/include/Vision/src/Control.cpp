#include <ctime>
#include <iostream>
#include <chrono>
#include <experimental/filesystem>

#include "Control.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <spdlog/sinks/basic_file_sink.h>

namespace vss {
Control::Control(const std::string& capturePath) 
    : m_capturePath(capturePath) {
    if(startInotify()) {
        m_failedToBuild = true;
    }

    m_logger = spdlog::basic_logger_mt("vision_log", "logs/vision_log.txt");
    spdlog::set_default_logger(m_logger);
    m_logger->set_pattern("[%D|%T] (%l) : %v");
    m_logger->flush_on(spdlog::level::info);
    
    m_logger->info("System Start.");

    m_processImages = std::make_unique<ProcessImages>(cameraQueue);

    //spawn threads
    m_processThread = std::thread(&ProcessImages::start, m_processImages.get());
    
    m_isRunning = true;
}


bool Control::startInotify() {
    boost::filesystem::path path(m_capturePath);
    auto handleNotification = [&](inotify::Notification notification) {
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
        m_logger->info(format("New robot [%d] added to allyRobots map.", t_id));

    } else {
        m_enemyRobots[t_id] = std::make_unique<Robot>(t_id, t_primaryColor, t_isAlly);
        m_logger->info("New robot [%d] added to enemyRobots map.", t_id);
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
    m_logger->info("'%1%' inserted in cameraQueue", path);
}


void Control::putInCommandQueue(const Command command) {
    m_logger->info(format("Captured command RobotId:%d.", command.robotId));
    commandQueue.put(PolyM::DataMsg<Command>(0, command));
}


position Control::getAllyPos(const id allyID){
    return m_allyRobots[allyID]->getPosition();
}


Control::~Control(){
    m_logger->info("System Stop.");
    m_isRunning = false;
    m_notifier.stop();

    m_processImages->stop();

    m_processThread.join();
    m_monitorThread.join();
}

}