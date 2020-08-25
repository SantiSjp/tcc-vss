#include <ctime>
#include <iostream>
#include <chrono>
#include <experimental/filesystem>

#include "Control.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

Control::Control(const std::string& capturePath) 
    : m_capturePath(capturePath) {
    if(startInotify()) {
        m_failedToBuild = true;
    }

    proc = std::make_unique<ProcessImages>(cameraQueue);

    //spawn threads
    processThread = std::thread(&ProcessImages::start, proc.get());
    
    isRunning = true;
}

bool Control::startInotify() {
    boost::filesystem::path path(m_capturePath);
    auto handleNotification = [&](inotify::Notification notification) {
        std::cout << notification.path.c_str() << std::endl;
        getImage(notification.path.c_str());
    };

    auto events = {inotify::Event::create, inotify::Event::moved_to, inotify::Event::move};

    m_notifier = inotify::BuildNotifier();
    m_notifier.watchPathRecursively(path);
    m_notifier.onEvents(events, handleNotification);

    m_monitorThread = std::thread([&](){m_notifier.run();});
    return true;
}


void Control::addRobot(const id mid, 
    const color mprimaryColor, 
    const bool misAlly) {
    
    if(misAlly) {
        allyRobots[mid] = std::make_unique<Robot>(mid, mprimaryColor, misAlly);
        
    } else {
        enemyRobots[mid] = std::make_unique<Robot>(mid, mprimaryColor, misAlly);
    }
}


void Control::getImage(const std::string& path){
        std::cout << "03" << std::endl;
        cv::Mat image;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        image = cv::imread(path, cv::ImreadModes::IMREAD_COLOR);
        if(!image.data){
            return;
        }
        std::cout << "04" << std::endl;
        cameraQueue.put(PolyM::DataMsg<cv::Mat>(0,image));
        std::cout << "05" << std::endl;
}

void Control::onNewFile(const callBack& t_callback){
    m_notifyCallBack = t_callback;
}

Control::~Control(){
    isRunning = false;
    m_notifier.stop();

    proc->stop();

    processThread.join();
    m_monitorThread.join();
}