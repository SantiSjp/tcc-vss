#include "gtest/gtest.h"
#include <Vision/src/Control.h>

namespace {
    using namespace vss;

    TEST(Robots, AddAlly) {
        Control control("/tmp/vision/capture");
        const id newId = 1;
        control.addRobot(newId, {0,1}, true);
        ASSERT_EQ(0, control.getAllyPos(newId)[0]);
        ASSERT_EQ(0, control.getAllyPos(newId)[1]);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    

}