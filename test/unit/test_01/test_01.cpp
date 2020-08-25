#include "gtest/gtest.h"
#include <Vision/Control.h>

namespace {

    TEST(Robots, AddAlly) {
        Control vision("/tmp/vision/capture");
        const id newId = 1;
        vision.addRobot(newId, {0,1}, true);
        ASSERT_EQ(true, vision.allyRobots[newId]->ally());
    }
    

}