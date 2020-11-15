#include "Field.h"

#include "DataTypes.h"

#include <iostream>
#include <fstream>

namespace vss {

Field::Field(const lenght& t_lenght, const std::vector<cv::Point>& fieldMargins) : fieldLenght(t_lenght) {
    gameMap = std::vector<std::vector<pos>>(fieldLenght[0]+1, std::vector<pos>(fieldLenght[1]+1, pos(STATE::EMPTY)));
    
    for (auto pixel : fieldMargins) {
        gameMap[pixel.y][pixel.x] = STATE::BORDER;
    }

    currentField = std::make_shared<cv::Mat>(gameMap.size(), gameMap.back().size(), CV_8UC3, cv::Scalar(150,150,150));
    std::vector<std::vector<cv::Point>> margin = {fieldMargins};
    cv::drawContours(*currentField, margin, 0, cv::Scalar(0, 0, 255), 0, 8);
}


void Field::updateField(const Element& element){
    cv::Mat dest;
    dest = currentField->operator()(cv::Rect(element.position[0], element.position[1], element.image.cols, element.image.rows));
    element.image.copyTo(dest);
    cv::imwrite("/tmp/vision/test-output/current_field.png", *currentField);
}


}