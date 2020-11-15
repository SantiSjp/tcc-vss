#include "Field.h"

#include "DataTypes.h"
#include "Robot.h"

namespace vss {

Field::Field(const lenght& t_lenght, const std::vector<cv::Point>& fieldMargins, 
    const std::string& logName, const std::string& logPath) : pictureLenght(t_lenght) {
    
    //Create logger:
    m_logger = std::make_unique<Logger>(logName, logPath, Level::debug);

    //Initialize game map:
    gameMap = std::vector<std::vector<pos>>(t_lenght[0]+1, std::vector<pos>(t_lenght[1]+1, pos(STATE::EMPTY)));
    for (auto pixel : fieldMargins) {
        gameMap[pixel.y][pixel.x] = STATE::BORDER;
    }

    //Initialize position:
    ballPos = {0, 0};

    //Create clean field picture:
    cleanField = std::make_shared<cv::Mat>(gameMap.size(), gameMap.back().size(), CV_8UC3, cv::Scalar(150,150,150));
    std::vector<std::vector<cv::Point>> margin = {fieldMargins};
    cv::drawContours(*cleanField, margin, 0, cv::Scalar(0, 0, 255), 0, 8);
}


void Field::updateField(const std::vector<Element>& elements){
    //Update elements:
    elementsOnField.clear();
    std::copy(elements.begin(), elements.end(), std::back_inserter(elementsOnField));

    //Update printable map:
    cv::Mat dest;
    currentField = cleanField->clone();
    for(auto element : elementsOnField) {
        dest = currentField(cv::Rect(element.position[0], element.position[1], element.image.cols, element.image.rows));
        element.image.copyTo(dest);
    }

    //Update Robots:
    for(auto element : elementsOnField) {
        
        if(element.isAlly) {
            if (allyRobots.count(element.ID) == 0) {
                allyRobots[element.ID] = std::make_unique<Robot>(element.ID, color(0,0), true);
            }
            allyRobots[element.ID]->updatePosition(element.position);

        } else if(element.isBall) {
            ballPos = element.position;

        } else {
            if (enemyRobots.count(element.ID) == 0) {
                enemyRobots[element.ID] = std::make_unique<Robot>(element.ID, color(0,0), false);
            }
            enemyRobots[element.ID]->updatePosition(element.position);
            
        }

    }

    m_logger->debug("Field updated");
}


void Field::printCurrentField(){
    cv::imwrite("/tmp/vision/test-output/current_field.png", currentField);
}


lenght Field::getPictureLenght() const {
    return pictureLenght;
}


std::vector<Element> Field::getElementPositions() const{
    return elementsOnField;
}


position Field::getAllyPosition(const id allyID) {
    position pos = {0,0};
    if(allyRobots.count(allyID) != 0){
        pos = allyRobots[allyID]->getPosition();
    }
    return pos;
}


position Field::getEnemyPosition(const id enemyID) {
    position pos = {0,0};
    if(enemyRobots.count(enemyID) != 0){
        pos = enemyRobots[enemyID]->getPosition();
    }
    return pos;
}


position Field::getBallPosition() {
    return ballPos;
}


}