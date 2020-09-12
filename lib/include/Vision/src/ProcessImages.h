#ifndef PROCESSIMAGES_H
#define PROCESSIMAGES_H

#include <memory>
#include <vector>
#include <string>

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

    struct ColorCalibration {
        cv::Mat fieldImage;
        cv::Mat ballColorImage;
        std::vector<cv::Mat> friendlyColorsImages;

        //cv::Mat allyMask;
        cv::Mat ballMask;

        cv::Scalar ballMaskThresholdLow;
        cv::Scalar ballMaskThresholdHigh;

    } calibration;

    void calibrate( const std::string& fieldImagePath,
                    const std::string& ballColorImagePath,
                    const std::vector<std::string>& friendlyColorsImagePath);

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