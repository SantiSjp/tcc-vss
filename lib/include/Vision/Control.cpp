#include <ctime>
#include <iostream>
#include <memory>
#include <chrono>
#include <string>
//#include <filesystem>
#include <experimental/filesystem>
#include <sys/inotify.h>

#include "Control.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <inotify-cpp/NotifierBuilder.h>

int Control::start(){
    //spawn threads
    imageThread = std::thread(&Control::getImage, this);

    isRunning = true;
    // while(isRunning){
    //     std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    //     std::cout << "running" << std::endl;
    // }

    return 0;
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


void Control::getImage(){
    while(isRunning) {
        //change with inotify
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        
        //check dir with inotify
        
        std::string path("/tmp/vision/capture");
        const auto& entry = std::experimental::filesystem::directory_iterator(path);
        //std::cout << entry->path() << std::endl;

        cv::Mat image;
        image = cv::imread(entry->path(), cv::ImreadModes::IMREAD_COLOR);
        if(!image.data){
            continue;
        }

        //test saving image in /tmp
        std::string output = "/tmp/vision/test-output/test-" + std::to_string(imageSeqNum) + ".png";
        imageSeqNum++;
        cv::imwrite(output, image);
    }
}

Control::~Control(){
    isRunning = false;
    imageThread.join();
}