#include "gtest/gtest.h"
#include "Vision/src/Ball.h"

namespace{
    using namespace vss;

    TEST(BallTest, CreateBall){
        std::vector<int> color = {1,2};
        position pos = {3,4};

        Ball ball(1.0,color,pos);
        ASSERT_EQ(ball.getRadius(),1.0);
        for(int i = 0; i<ball.getColor().size(); i++){
            ASSERT_EQ(ball.getColor()[i],color[i]);
            ASSERT_EQ(ball.getPosition()[i],pos[i]);
        }
    }

}