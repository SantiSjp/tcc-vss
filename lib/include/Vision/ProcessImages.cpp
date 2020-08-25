#include <iostream>

#include "ProcessImages.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>

ProcessImages::ProcessImages(PolyM::Queue& queue) 
    : cameraQueue(queue) {

}

void ProcessImages::start(){
    isRunning = true;
    while (isRunning){
        std::cout << "00" << std::endl;
        auto codedImage = cameraQueue.get(500);
        std::cout << "01" << std::endl;
        if(codedImage->getMsgId() != PolyM::MSG_TIMEOUT){
            auto image = dynamic_cast<PolyM::DataMsg<cv::Mat>&>(*codedImage).getPayload();
            std::cout << "02" << std::endl;
            //Apply all image filters needed with opencv

            //test saving image in /tmp
            std::string output = "/tmp/vision/test-output/test-" + std::to_string(imageSeqNum) + ".png";
            std::cout << output << std::endl;
            imageSeqNum++;
            cv::imwrite(output, image);
        }
    }

}

void ProcessImages::stop(){
    isRunning = false;
}