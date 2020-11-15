#include <iostream>
#include <vector>
#include <array>
#include <tuple>

#include "ProcessImages.h"

#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/utility.hpp"

namespace vss {

ProcessImages::ProcessImages(PolyM::Queue& t_cameraQ, PolyM::Queue& t_processQ, 
    const std::string& logName, const std::string& logPath) 
    : m_cameraQueue(t_cameraQ), m_processQueue(t_processQ) {
    
    m_logger = std::make_unique<Logger>(logName, logPath, Level::debug);
    m_logger->info("Starting ProcessImages thread");

    calibrate(  "/tmp/vision/test/field/vss_field_clean.png",
                "/tmp/vision/test/color/ball_color.png",
                {"/tmp/vision/test/color/ally_color_01.png"});
    
}

void ProcessImages::calibrate(  const std::string& fieldImagePath,
                                const std::string& ballColorImagePath,
                                const std::vector<std::string>& friendlyColorsImagePath) {
    
    fieldImage = cv::imread(fieldImagePath, cv::ImreadModes::IMREAD_COLOR);
    
    int allyColorNum = 1;
    for (auto& path : friendlyColorsImagePath) {
        auto image = cv::imread(path, cv::ImreadModes::IMREAD_COLOR);
        cv::cvtColor(image, image, cv::COLOR_BGR2HSV);
        cv::Scalar average = cv::mean(image);
        Mask newMask;
        newMask.thresholdLow =  cv::Scalar( average.val[0]-10 > 0? average.val[0]-10 : 0,
                                            100, 
                                            20);
        
        newMask.thresholdHigh = cv::Scalar( average.val[0]+10 > 255? 255 : average.val[0]+10,
                                            255,
                                            
                                            255);
        allyColorMask.emplace_back(newMask);
        m_logger->debug("Ally Color [%d], path: '%s', H:%d S:%d V:%d", allyColorNum, path, average.val[0], average.val[1], average.val[2]);
        allyColorNum++;
    }

    auto ballColorImage = cv::imread(ballColorImagePath, cv::ImreadModes::IMREAD_COLOR);
    cv::cvtColor(ballColorImage, ballColorImage, cv::COLOR_BGR2HSV);
    cv::Scalar average = cv::mean(ballColorImage);
    ballColorMask.thresholdLow = cv::Scalar(average.val[0]-5, 100, 20);
    ballColorMask.thresholdHigh = cv::Scalar(average.val[0]+5, 255, 255);
    m_logger->debug("Ball Color, path: '%s',  H:%d S:%d V:%d", ballColorImagePath, average.val[0], average.val[1], average.val[2]);

}


std::vector<Element> ProcessImages::extractImageInfo(cv::Mat& image) {
    std::vector<Element> elements;
    cv::Mat diffImage;
    cv::subtract(image, fieldImage, diffImage);
   
    cv::Mat grayScaleImage;
    cv::cvtColor(diffImage, grayScaleImage, cv::COLOR_BGR2GRAY);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(grayScaleImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    for (size_t idx = 0; idx < contours.size(); idx++) {
        auto img = image.clone();
        auto rect = cv::boundingRect(contours[idx]);
        if (rect.area() < 700){
            continue;
        }
        cv::Rect crop(rect.x,rect.y, rect.width, rect.height);
        cv::Mat cropped = img(crop);
        Element newElement (cropped, {rect.x+rect.width/2, rect.y+rect.height/2});
        elements.emplace_back(newElement);
        //cv::circle(image, {newElement.position[0], newElement.position[1]}, 3.0, cv::Scalar(0,0,255 ), 1, 8 );
        //cv::imwrite("/tmp/vision/test-output/centros.png", image);
    }
    
    //Identify objects as allys, enemies or ball
    int image_num = -1;
    for (auto& element : elements) {
        image_num++;
        cv::Mat img;
        cv::cvtColor(element.image, img, cv::COLOR_BGR2HSV);
    
        cv::Mat allyMask, allyColor1, allyColor2;
        cv::inRange(img, allyColorMask[0].thresholdLow, allyColorMask[0].thresholdHigh, allyMask);
        //cv::inRange(img, allyColorMask[1].thresholdLow, allyColorMask[1].thresholdHigh, allyColor2);
        //allyMask = cv::max(allyColor1, allyColor2);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(allyMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
        if(contours.size() == 1) {
            element.isAlly = true;
            m_logger->debug("Found Ally  - Center {%d,%d}", element.position[0], element.position[1]);
            continue;
        }
       
        cv::Mat ballMask;
        cv::inRange(img, ballColorMask.thresholdLow, ballColorMask.thresholdHigh, ballMask);      
        cv::findContours(ballMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
        if(contours.size() == 1) {
            element.isBall = true;
            m_logger->debug("Found Ball  - Center {%d,%d}", element.position[0], element.position[1]);
            continue;
        }

        m_logger->debug("Found Enemy - Center {%d,%d} ", element.position[0], element.position[1]);
    }

    //Write image files for all objects
    // for (auto element : elements) {
    //     std::string ally = element.isAlly? "ally": element.isBall? "ball" : "enemy";
    //     const std::string output {"/tmp/vision/test-output/" + ally + "-" + std::to_string(imageSeqNum) + ".png"};
    //     /cv::imwrite(output, element.image);
    //     imageSeqNum++;
    // }

    return elements;
}


void ProcessImages::start(){
    isRunning = true;
    while (isRunning){
        auto codedImage = m_cameraQueue.get(500);
        if(codedImage->getMsgId() != PolyM::MSG_TIMEOUT){
            auto image = dynamic_cast<PolyM::DataMsg<cv::Mat>&>(*codedImage).getPayload();
            const auto imageElements = extractImageInfo(image);
            m_logger->debug("Processed image, %d elements.", imageElements.size());
            m_processQueue.put(PolyM::DataMsg<std::vector<Element>>(0, imageElements));
        }
    }

}

void ProcessImages::stop(){
    m_logger->debug("Finishing");
    isRunning = false;
}

}