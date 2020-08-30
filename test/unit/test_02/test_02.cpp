#include "gtest/gtest.h"
#include "Vision/src/Control.h"
#include "Vision/src/Command.h"

namespace {
    using namespace vss;

    TEST(CommandRobotTest, CreateCommand) {
        const auto cmd = CommandFactory::makeNew(1,1,2);
        ASSERT_EQ(1, cmd.robotId);
        ASSERT_EQ(1, cmd.moveToPos[0]);
        ASSERT_EQ(2, cmd.moveToPos[1]);
    }
    

    // TEST(CommandRobotTest, putCommand) {
    //     Control commandTest("/tmp/vision/capture");
    //     Command cmd(1,{1,2});

    //     commandTest.putCommand(cmd);

    //     auto cmd = commandTest.commandQueue.get(500);
    //     auto aux = dynamic_cast<PolyM::DataMsg<std::map<int,std::vector<double>>>&>(*cmd).getPayload();
    //     ASSERT_EQ(1,aux[1][0]);
    //     ASSERT_EQ(2,aux[1][1]);
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // }
    
}