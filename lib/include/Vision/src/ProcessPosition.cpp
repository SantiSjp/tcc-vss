#include "ProcessPosition.h"
#include "DataTypes.h"

#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/utility.hpp"

namespace vss {

ProcessPosition::ProcessPosition(PolyM::Queue& t_processQ,
    const std::string& logName, const std::string& logPath) : m_processQueue(t_processQ) {
    
    m_logger = std::make_unique<Logger>(logName, logPath, Level::debug);
    m_logger->info("Starting ProcessPositon thread");

    configure();
}

ProcessPosition::~ProcessPosition() {
    isRunning = false;
}


void ProcessPosition::configure() {
    auto fieldImage = cv::imread("/tmp/vision/test/field/vss_field_clean.png", cv::ImreadModes::IMREAD_GRAYSCALE);
    auto fieldOutline = cv::imread("/tmp/vision/test/field/vss_field_outline.png", cv::ImreadModes::IMREAD_GRAYSCALE);

    //Field Contours:
    std::vector<std::vector<cv::Point>> fieldsContours;
    cv::threshold(fieldImage, fieldImage, 100, 255, cv::THRESH_BINARY_INV);
    cv::findContours(fieldImage, fieldsContours, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE);

    //Field Outline:
    std::vector<std::vector<cv::Point>> outlineContours;
    cv::threshold(fieldOutline, fieldOutline, 100, 255, cv::THRESH_BINARY);
    cv::findContours(fieldOutline, outlineContours, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE);
    int outlineCIndex = 0;
    for (int i = 0; i < outlineContours.size(); i++) {
        outlineCIndex = outlineContours[i].size() > outlineContours[outlineCIndex].size() ?  i : outlineCIndex;
    }

    //Compare Contours
    double bestFit = 0;
    double bestResutl = 1;
    //cv::Mat contourImage(fieldImage.rows, fieldImage.cols, CV_8UC1, cv::Scalar::all(0));
    for (size_t idx = 0; idx < fieldsContours.size(); idx++) {
        auto result = cv::matchShapes(outlineContours[outlineCIndex], fieldsContours[idx], cv::CONTOURS_MATCH_I1, 0);
        if(result < bestResutl) {
            bestResutl = result;
            bestFit = idx;
        }
    }

    //Initialize field object:
    const lenght fieldSize = {fieldImage.rows, fieldImage.cols};
    currentField = std::make_unique<Field>(fieldSize, fieldsContours[bestFit]);
    m_logger->debug("Field Size: %dx%d pixels", currentField->getLenght()[0], currentField->getLenght()[1]);

    // for(auto pixel : fieldsContours[bestFit]) {
    //     m_logger->debug("{%d,%d}", pixel.x, pixel.y);
    // }

    //cv::drawContours(contourImage, fieldsContours, bestFit, cv::Scalar(254, 100, 100), 0, 8);
    //cv::imwrite("/tmp/vision/test-output/contourImage.png", contourImage);
}


void ProcessPosition::start(){
    isRunning = true;
    while (isRunning){
        auto queueElement = m_processQueue.get(500);
        if(queueElement->getMsgId() != PolyM::MSG_TIMEOUT){
            const auto rawPosition = dynamic_cast<PolyM::DataMsg<std::vector<Element>>&>(*queueElement).getPayload();
            calculateCurrentPosition(rawPosition);
        }
    }

}


void ProcessPosition::stop() {
    m_logger->debug("Finishing");
    isRunning = false;
}


void ProcessPosition::calculateCurrentPosition(const std::vector<Element>& rawPosition) {
    for (auto& element : rawPosition) {
        m_logger->debug("Calculating position...");
        currentField->updateField(element);
    }
}


}