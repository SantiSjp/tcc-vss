#ifndef FIELD_H
#define FIELD_H

#include <string>
#include <vector>
#include <memory>

#include "DataTypes.h"

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
    const lenght fieldLenght;
    std::vector<std::vector<pos>> gameMap;

    std::shared_ptr<cv::Mat> currentField;

public:
    Field(const lenght& t_lenght, const std::vector<cv::Point>& fieldMargins);

    void updateField(const Element& element);
    lenght getLenght() const {return fieldLenght;}

};

}

#endif // !FIELD_H