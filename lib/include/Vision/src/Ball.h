#ifndef BALL_H
#define BALL_H

#include "DataTypes.h"

namespace vss{
    class Ball{
    private:
        double ballRadius;
        color ballColor;
        position ballPosition;

    public:
        Ball(   double mballRadius, 
                color mballColor, 
                position mballPosition)
                    :ballRadius(mballRadius), ballColor(mballColor), ballPosition(mballPosition) {};
    
        position getPosition(){return ballPosition;}
        color getColor(){return ballColor;}
        double getRadius(){return ballRadius;}
    };
}

#endif // !BALL_H
