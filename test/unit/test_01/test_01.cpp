#include "gtest/gtest.h"
#include "Vision/Control.h"

namespace {

    TEST(Robots, AddAlly) {
        Control control;
        control.start();
        const id newId = 1;
        control.addRobot(newId, {0,1}, true);
        ASSERT_EQ(true, control.allyRobots[newId]->ally());
    }

}