#include <iostream>
#include <vector>
#include <array>
#include <tuple>

#include "ProcessImages.h"

#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/utility.hpp"

namespace vss {

ProcessImages::ProcessImages(PolyM::Queue& queue) 
    : cameraQueue(queue) {
    fieldImage = cv::imread("/tmp/vision/test-field/vss_field_clean.png", cv::ImreadModes::IMREAD_COLOR);
}

std::vector<Element> ProcessImages::extractImageInfo(cv::Mat& image) {
    std::vector<Element> elements;

    //Convert Image to grayscale and find contours
    cv::Mat grayScaleImage;
    cv::cvtColor(image, grayScaleImage, cv::COLOR_BGR2GRAY);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(grayScaleImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    
    //Find and isolate image elements
    for (size_t idx = 0; idx < contours.size(); idx++) {
        auto img = image.clone();
        auto rect = cv::boundingRect(contours[idx]);
        cv::rectangle(img, {rect.x,rect.y}, {rect.x+rect.width,rect.y+rect.height}, (0,0,255), 2);
        cv::Rect crop(rect.x,rect.y, rect.width, rect.height);
        cv::Mat cropped = img(crop);
        Element newElement (cropped, {rect.x+rect.width/2, rect.y+rect.height/2}); 
        elements.emplace_back(newElement);
        cv::circle(image, {newElement.position[0], newElement.position[1]}, 3.0, cv::Scalar(0,0,255 ), 1, 8 );
    }
    
    //Identify objects as allys, enemies or ball
    for (auto& element : elements) {
        cv::Mat img;
        cv::cvtColor(element.image, img, cv::COLOR_BGR2HSV);
        
        cv::Mat blueMask, greenMask;
        cv::inRange(img, cv::Scalar(130, 160, 120), cv::Scalar(180, 255, 255), greenMask);
        cv::inRange(img, cv::Scalar(0, 100, 100), cv::Scalar(40, 255, 255), blueMask);
        cv::Mat allyMask = blueMask + greenMask;
        
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(allyMask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
        if(contours.size() > 0) {
            //std::cout << "Achou aliado" << std::endl;
            element.isAlly = true;
            continue;
        }

        cv::Mat orangeMask;
        cv::inRange(img, cv::Scalar(100, 160, 120), cv::Scalar(120, 255, 255), orangeMask);
        cv::findContours(orangeMask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_NONE); 
        const std::string output {"/tmp/vision/test-output/orangeMask.png"};
        cv::imwrite(output, orangeMask);
        if(contours.size() > 0) {
            //std::cout << "Achou bola" << std::endl;
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
        auto codedImage = cameraQueue.get(500);
        if(codedImage->getMsgId() != PolyM::MSG_TIMEOUT){
            auto image = dynamic_cast<PolyM::DataMsg<cv::Mat>&>(*codedImage).getPayload();
            
            cv::Mat diffImage;
            cv::subtract(fieldImage, image, diffImage);
            const auto imageElements = extractImageInfo(diffImage);

            //test saving image in /tmp
            //const std::string output {"/tmp/vision/test-output/centers-" + std::to_string(imageSeqNum) + ".png"};
            //cv::imwrite(output, colourImage);
            imageSeqNum++;
        }
    }

}

void ProcessImages::stop(){
    isRunning = false;
}

}