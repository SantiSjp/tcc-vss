#ifndef ROBOT_H
#define ROBOT_H

#include <map>
#include <vector>

using position = std::map<int, int>;
using color = std::vector<int>;
using id = int;

class Robot {
private:
    const int id;
    const bool isAlly; 
    const std::vector<int> primaryColor;
    
    position currentPosition;

public:
    Robot(  const int mid, 
            const color mcolor, 
            const bool misAlly = false) 
                : id(mid), primaryColor(mcolor), isAlly(misAlly) {};

    void updatePosition(position& pos) { currentPosition = pos;}
    const position getPosition() const { return currentPosition; }

};

#endif