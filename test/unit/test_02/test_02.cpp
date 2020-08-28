#include "gtest/gtest.h"
#include "Vision/CommandRobot.h"

namespace {

    TEST(CommandRobotTest, addCommand) {
        CommandRobot commandTest("/tmp/vision/capture");
        commandTest.insertCommandInQueue(1,{1,2});

        auto cmd = commandTest.commandQueue.get(500);
        auto aux = dynamic_cast<PolyM::DataMsg<std::map<int,std::vector<double>>>&>(*cmd).getPayload();
        ASSERT_EQ(1,aux[1][0]);
        ASSERT_EQ(2,aux[1][1]);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
}