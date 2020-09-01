#ifndef PROCESSIMAGES_H
#define PROCESSIMAGES_H

#include <memory>

#include "PolyM/Queue.h"

#include <opencv2/core/core.hpp>

class ProcessImages {
private:
    bool isRunning = false;
    int imageSeqNum = 0;

    cv::Mat fieldImage;     //temporary

    PolyM::Queue& cameraQueue;

    cv::Mat extractImageInfo(cv::Mat& image);

public:
    ProcessImages(PolyM::Queue& queue);

    void start();
    void stop();

};

#endif