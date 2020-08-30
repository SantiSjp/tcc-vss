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
        const color primaryColor;
        
        position currentPosition;

    public:
        Robot(  const int mid, 
                const color mcolor, 
                const bool misAlly = false,
                const position pos = {0,0})
                    : m_id(mid), primaryColor(mcolor), isAlly(misAlly), currentPosition(pos) {};

        void updatePosition(const position& pos) { currentPosition = pos;}
        const position getPosition() const { return currentPosition; }
        bool ally() const {return isAlly;}
    };
}

#endif