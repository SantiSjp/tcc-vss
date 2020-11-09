#include "gtest/gtest.h"
#include "Vision/src/ProcessImages.h"

#include <thread>
#include <experimental/filesystem>

#include <PolyM/Queue.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {
    using namespace vss;
    
    class TestProcessImage : public ::testing::Test {
    public:
        PolyM::Queue cameraQ, processQ;
        std::unique_ptr<ProcessImages> proc;
        cv::Mat imageForm01;
        
        TestProcessImage() {
                imageForm01 = cv::imread("/tmp/vision/test/frames/frame_02.png", cv::ImreadModes::IMREAD_COLOR);
                proc = std::make_unique<ProcessImages>(cameraQ, processQ, "unit_test", "unit_test_log.txt");
        };

    };


    TEST_F(TestProcessImage, NumberOfElements) {
        const auto elements = proc->extractImageInfo(imageForm01);        
        ASSERT_EQ(7, elements.size());
    }
   
    TEST_F(TestProcessImage, NumberOfEachElements) {
        int qntAllys = 0;
        int qntEnemies = 0;
        int qntBalls = 0;
        const auto elements = proc->extractImageInfo(imageForm01);
        for (auto element : elements) {
            if (element.isAlly) {
                qntAllys++;
            
            } else if(element.isBall) {
                qntBalls++;
            
            } else {
                qntEnemies++;
            }

        }

        ASSERT_EQ(3, qntAllys);
        ASSERT_EQ(3, qntEnemies);
        ASSERT_EQ(1, qntBalls);
    }

    TEST_F(TestProcessImage, PositionOfAnElement) {
        const auto elements = proc->extractImageInfo(imageForm01);        
        ASSERT_EQ(712, elements[0].position[0]);
        ASSERT_EQ(727, elements[0].position[1]);
    }

}