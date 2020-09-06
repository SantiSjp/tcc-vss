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
        auto codedImage = cameraQueue.get(500);
        if(codedImage->getMsgId() != PolyM::MSG_TIMEOUT){
            auto image = dynamic_cast<PolyM::DataMsg<cv::Mat>&>(*codedImage).getPayload();
            //Apply all image filters needed with opencv

            //test saving image in /tmp
            std::string output = "/tmp/vision/test-output/test-" + std::to_string(imageSeqNum) + ".png";
            imageSeqNum++;
            cv::imwrite(output, image);
        }
    }

}

void ProcessImages::stop(){
    isRunning = false;
}