#ifndef PROCESSIMAGES_H
#define PROCESSIMAGES_H

#include <memory>

#include "DataTypes.h"
#include "Logger.h"

#include "PolyM/Queue.h"
#include <opencv2/core/core.hpp>

namespace vss {
class ProcessImages {
private:
    bool isRunning = false;
    int imageSeqNum = 0;

    std::unique_ptr<Logger> m_logger;

    cv::Mat fieldImage;     //temporary
    cv::Mat ballColor;      //temporary

    PolyM::Queue& m_cameraQueue;
    PolyM::Queue& m_processQueue;

public:
    ProcessImages(PolyM::Queue& t_cameraQ, 
                    PolyM::Queue& t_processQ,
                    const std::string& logName = "ProcessImages", 
                    const std::string& logPath = "logs/vision_log.txt");

    void start();
    void stop();

    std::vector<Element> extractImageInfo(cv::Mat& image);

};
}

#endif