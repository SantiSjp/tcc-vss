#include "gtest/gtest.h"
#include <Vision/src/Control.h>

namespace {
    using namespace vss;

    class TestFixture : public ::testing::Test {
        public:
            Control control;

            TestFixture(const std::string& path = "/tmp/vision/capture")
                : control(path) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                };
    };


    TEST_F(TestFixture, AddAlly) {
        const id newId = 1;
        control.addRobot(newId, {0,1}, true);
        ASSERT_EQ(0, control.getAllyPos(newId)[0]);
        ASSERT_EQ(0, control.getAllyPos(newId)[1]);
    }
    
    

}