#include "gtest/gtest.h"
#include "Vision/CommandRobot.h"

namespace {

    TEST(CommandRobotTest, addCommand) {
        CommandRobot commandTest;
        commandTest.control.start();
        commandTest.insertCommandInQueue(1,{1,2});

        auto cmd = commandTest.control.commandQueue.get(500);
        auto aux = dynamic_cast<PolyM::DataMsg<std::map<int,std::vector<double>>>&>(*cmd).getPayload();
        ASSERT_EQ(1,aux[1][0]);
        ASSERT_EQ(2,aux[1][1]);
    }
    
}