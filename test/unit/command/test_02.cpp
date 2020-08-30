#include "gtest/gtest.h"
#include "Vision/src/Command.h"

namespace {
    using namespace vss;
    
    TEST(CommandRobotTest, CreateCommand) {
        const position pos({10,15});
        Command cmd(2, pos);
        ASSERT_EQ(2, cmd.robotId);
        ASSERT_EQ(10, cmd.moveToPos[0]);
        ASSERT_EQ(15, cmd.moveToPos[1]);
    }
    
    TEST(CommandRobotTest, CommandFactory) {
        const auto cmd = CommandFactory::makeNew(1,1,2);
        ASSERT_EQ(1, cmd.robotId);
        ASSERT_EQ(1, cmd.moveToPos[0]);
        ASSERT_EQ(2, cmd.moveToPos[1]);
    }
   
}