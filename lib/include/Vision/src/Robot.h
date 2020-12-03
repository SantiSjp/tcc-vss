#ifndef ROBOT_H
#define ROBOT_H

#include <map>
#include <vector>

#include "DataTypes.h"

namespace vss {
    class Robot {
    private:
        const id m_id;
        const bool isAlly; 
        const cv::Mat image;
        
        position currentPosition;

    public:
        Robot(  const int mid, 
                const cv::Mat mimage, 
                const bool misAlly = false,
                const position pos = {0,0})
                    : m_id(mid), image(mimage), isAlly(misAlly), currentPosition(pos) {};

        void updatePosition(const position& pos) { currentPosition = pos;}
        const position getPosition() const { return currentPosition; }
        const cv::Mat getImage() const {return image;}
        bool ally() const {return isAlly;}
    };
}

#endif