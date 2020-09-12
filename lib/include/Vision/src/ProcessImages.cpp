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
                "/tmp/vision/test/color/cor_bola.png",
                {});
    
}

void ProcessImages::calibrate(  const std::string& fieldImagePath,
                                const std::string& ballColorImagePath,
                                const std::vector<std::string>& friendlyColorsImagePath) {
    
    calibration.fieldImage = cv::imread(fieldImagePath, cv::ImreadModes::IMREAD_COLOR);
    calibration.ballColorImage = cv::imread(ballColorImagePath, cv::ImreadModes::IMREAD_COLOR);

    cv::Mat hsvBall = calibration.ballColorImage.clone();
    cv::cvtColor(calibration.ballColorImage, hsvBall, cv::COLOR_BGR2HSV);
    cv::Scalar average = cv::mean(hsvBall);
    calibration.ballMaskThresholdLow = (average.val[0]-3, average.val[1]-10, average.val[2]-10);
    calibration.ballMaskThresholdHigh = (average.val[0]+3, 255, 255);
    m_logger->debug("HSVBall avarage H:%d S:%d V:%d", average.val[0], average.val[1], average.val[2]);
}


std::vector<Element> ProcessImages::extractImageInfo(cv::Mat& image) {
    std::vector<Element> elements;
    cv::Mat diffImage;
    cv::subtract(calibration.fieldImage, image, diffImage);
    
    //Convert Image to grayscale and find contours
    cv::Mat grayScaleImage;
    cv::cvtColor(diffImage, grayScaleImage, cv::COLOR_BGR2GRAY);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(grayScaleImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    
    //Find and isolate image elements
    for (size_t idx = 0; idx < contours.size(); idx++) {
        auto img = diffImage.clone();
        auto rect = cv::boundingRect(contours[idx]);
        cv::Rect crop(rect.x,rect.y, rect.width, rect.height);
        cv::Mat cropped = img(crop);
        Element newElement (cropped, {rect.x+rect.width/2, rect.y+rect.height/2}); 
        elements.emplace_back(newElement);
        cv::circle(image, {newElement.position[0], newElement.position[1]}, 3.0, cv::Scalar(0,0,255 ), 1, 8 );
    }
    
    int image_num = 0;
    //Identify objects as allys, enemies or ball
    for (auto& element : elements) {
        cv::Mat img;
        cv::cvtColor(element.image, img, cv::COLOR_BGR2HSV);
        
        cv::Mat blueMask, greenMask;
        cv::inRange(img, cv::Scalar(130, 160, 120), cv::Scalar(180, 255, 255), greenMask);
        cv::inRange(img, cv::Scalar(0, 100, 100), cv::Scalar(40, 255, 255), blueMask);
        auto allyMask = blueMask + greenMask;

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(allyMask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
        if(contours.size() > 0) {
            element.isAlly = true;
            continue;
        }

        //cv::inRange(img, cv::Scalar(100, 160, 255), cv::Scalar(120, 255, 255), ballMask);
        cv::inRange(img, calibration.ballMaskThresholdLow, calibration.ballMaskThresholdHigh, calibration.ballMask);        
        cv::findContours(calibration.ballMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
        cv::imwrite("/tmp/vision/test-output/ballMask" + std::to_string(image_num) + ".png", calibration.ballMask);
        image_num++;
        if(contours.size() > 0) {
            element.isBall = true;
            continue;
        }

       
    }

    //Write image files for all objects
    for (auto element : elements) {
        std::string ally = element.isAlly? "ally": element.isBall? "ball" : "enemy";
        const std::string output {"/tmp/vision/test-output/" + ally + "-" + std::to_string(imageSeqNum) + ".png"};
        cv::imwrite(output, element.image);
        imageSeqNum++;
    }

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