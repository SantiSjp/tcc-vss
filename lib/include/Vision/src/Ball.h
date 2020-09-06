#ifndef BALL_H
#define BALL_H

#include "DataTypes.h"

namespace vss{
    class Ball{
    private:
        const double ballRadius;
        const color ballColor;
        const position ballPosition;

    public:
        Ball(   const double mballRadius, 
                const color mballColor, 
                const position mballPosition)
                    :ballRadius(mballRadius), ballColor(mballColor), ballPosition(mballPosition) {};
    
        position getPosition() const {return ballPosition;}
        color getColor() const {return ballColor;}
        double getRadius()const {return ballRadius;}
    };
}

#endif // !BALL_H
