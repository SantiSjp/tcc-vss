#ifndef FIELD_H
#define FIELD_H

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "DataTypes.h"
#include "Robot.h"
#include "Logger.h"

#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/utility.hpp"
#include <opencv2/core/types.hpp>

namespace vss{

enum class STATE{
    EMPTY = 0,
    BORDER,
    GOAL,
    BALL,
    ENEMY,
    ALLY,
    OUT_SIDE
};


struct pos {
    pos(const STATE currentState) : state(currentState){}
    
    id objectID = 0;
    STATE state;
    
};


class Field {
private:
    //Game map:
    std::vector<std::vector<pos>> gameMap;

    // Printable map:
    std::shared_ptr<cv::Mat> cleanField;
    cv::Mat currentField;
    const lenght pictureLenght;

    //Position maps
    std::map<id, std::unique_ptr<Robot>> allyRobots;
    std::map<id, std::unique_ptr<Robot>> enemyRobots;
    position ballPos;

    //Elements
    std::vector<Element> elementsOnField;

    //Logger:
    std::unique_ptr<Logger> m_logger;

public:
    Field(const lenght& t_lenght, const std::vector<cv::Point>& fieldMargins, 
        const std::string& logName, const std::string& logPath);

    void updateField(const std::vector<Element>& elements);
    void printCurrentField();
    
    lenght getPictureLenght() const; 
    cv::Mat& getCleanField();

    std::vector<Element> getElementPositions() const;

    Robot& getAlly(const id allyID);
    Robot& getEnemy(const id enemyID);
    position getBallPosition();

};

}

#endif // !FIELD_H