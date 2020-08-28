#include "gtest/gtest.h"
#include <Vision/CommandRobot.h>

namespace {

    TEST(Robots, AddAlly) {
        CommandRobot commandTest("/tmp/vision/capture");
        const id newId = 1;
        commandTest.addRobot(newId, {0,1}, true);
        ASSERT_EQ(true, commandTest.allyRobots[newId]->ally());
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    

}