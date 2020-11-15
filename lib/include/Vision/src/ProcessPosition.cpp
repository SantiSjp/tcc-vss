#include "ProcessPosition.h"
#include "DataTypes.h"

#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/utility.hpp"
#include <opencv2/core.hpp>

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
    for (size_t idx = 0; idx < fieldsContours.size(); idx++) {
        auto result = cv::matchShapes(outlineContours[outlineCIndex], fieldsContours[idx], cv::CONTOURS_MATCH_I1, 0);
        if(result < bestResutl) {
            bestResutl = result;
            bestFit = idx;
        }
    }

    //Initialize field object:
    const lenght fieldSize = {fieldImage.rows, fieldImage.cols};
    currentField = std::make_unique<Field>(fieldSize, fieldsContours[bestFit], "Field", "logs/vision_log.txt");
    m_logger->debug("Field Size: %dx%d pixels", currentField->getPictureLenght()[0], currentField->getPictureLenght()[1]);
}


void ProcessPosition::start(){
    isRunning = true;
    while (isRunning){
        auto queueElement = m_processQueue.get(500);
        if(queueElement->getMsgId() != PolyM::MSG_TIMEOUT){
            const auto rawPosition = dynamic_cast<PolyM::DataMsg<std::vector<Element>>&>(*queueElement).getPayload();
            currentField->updateField(rawPosition);
            m_logger->debug("updated field");
            currentField->printCurrentField();
        }
    }

}


void ProcessPosition::stop() {
    m_logger->debug("Finishing");
    isRunning = false;
}


void ProcessPosition::calculateCurrentPosition(const std::vector<Element>& rawPosition) {
}

bool ProcessPosition::calculateFuturePosition(const Command& newCommand) { 
    cv::Mat fieldMargins = currentField->getCleanField().clone();
    auto robot = currentField->getAlly(newCommand.robotId);

    try{
        cv::Mat dest;
        dest = fieldMargins(cv::Rect(newCommand.moveToPos[0], newCommand.moveToPos[1], robot.getImage().cols, robot.getImage().rows));
        robot.getImage().copyTo(dest);
    
    } catch(cv::Exception e) {
        m_logger->debug("Command invalid!");
        return false;
    }
    
    m_logger->debug("Command validated");
    return true;
}

Field& ProcessPosition::getCurrentField() const{
    return *currentField;
}
    
}