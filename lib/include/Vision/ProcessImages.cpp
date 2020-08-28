#include <iostream>

#include "ProcessImages.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>

ProcessImages::ProcessImages(PolyM::Queue& queue) 
    : cameraQueue(queue) {

}

void ProcessImages::start(){
    isRunning = true;
    std::cout << "Start process Image" << std::endl;
    while (isRunning){
        auto codedImage = cameraQueue.get(500);
        std::cout << "Running" << std::endl;
        if(codedImage->getMsgId() != PolyM::MSG_TIMEOUT){
            auto image = dynamic_cast<PolyM::DataMsg<cv::Mat>&>(*codedImage).getPayload();
            //Apply all image filters needed with opencv

            //test saving image in /tmp
            std::string output = "/tmp/vision/test-output/test-" + std::to_string(imageSeqNum) + ".png";
            std::cout << output << std::endl;
            imageSeqNum++;
            cv::imwrite(output, image);
        }
    }
    std::cout << "Exit process_image::start()" << std::endl;

}

void ProcessImages::stop(){
    std::cout << "Stopping process_image" << std::endl;
    isRunning = false;
}