#include <iostream>
#include <vector>

#include "ProcessImages.h"

#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc/imgproc.hpp"

ProcessImages::ProcessImages(PolyM::Queue& queue) 
    : cameraQueue(queue) {
    fieldImage = cv::imread("/tmp/vision/test/vss_field_clean.png", cv::ImreadModes::IMREAD_COLOR);
}


cv::Mat ProcessImages::extractImageInfo(cv::Mat& image) {
    cv::Mat diffImage;
    cv::subtract(fieldImage, image, diffImage);
    cv::imwrite("/tmp/vision/test-output/diff-" + std::to_string(imageSeqNum) + ".png", diffImage);

    //configure image to apply findContours
    cv::cvtColor(diffImage, diffImage, cv::COLOR_BGR2GRAY);
    cv::imwrite("/tmp/vision/test-output/grayscale-" + std::to_string(imageSeqNum) + ".png", diffImage);
    //cv::threshold(diffImage, diffImage, 150, 255, cv::THRESH_OTSU);
    //cv::imwrite("/tmp/vision/test-output/threshold-" + std::to_string(imageSeqNum) + ".png", diffImage);

    cv::Mat contourOutput = diffImage.clone();
    
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(contourOutput, contours, cv::RETR_TREE, cv::CHAIN_APPROX_NONE); // CHAIN_APPROX_NONE -> all the points in the contour
    
    cv::Mat contourImage(diffImage.size(), CV_8UC1, cv::Scalar(0,0,0));
    std::cout << "Contours: " << contours.size() << std::endl;
    for (size_t idx = 0; idx < contours.size(); idx++) {
        cv::drawContours(contourImage, contours, idx, cv::Scalar(255, 0, 0));
    }
    
    return contourImage;
}


void ProcessImages::start(){
    isRunning = true;
    while (isRunning){
        auto codedImage = cameraQueue.get(500);
        if(codedImage->getMsgId() != PolyM::MSG_TIMEOUT){
            auto image = dynamic_cast<PolyM::DataMsg<cv::Mat>&>(*codedImage).getPayload();
            //Apply all image filters needed with opencv
            const auto processedImage = extractImageInfo(image);                        

            //test saving image in /tmp
            const std::string output {"/tmp/vision/test-output/test-" + std::to_string(imageSeqNum) + ".png"};
            cv::imwrite(output, processedImage);
            imageSeqNum++;
        }
    }

}

void ProcessImages::stop(){
    isRunning = false;
}