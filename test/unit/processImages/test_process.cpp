#include "gtest/gtest.h"
#include "Vision/src/ProcessImages.h"

#include <thread>

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
                imageForm01 = cv::imread("/tmp/vision/test/frames/vss_field_form_01.png", cv::ImreadModes::IMREAD_COLOR);
                proc = std::make_unique<ProcessImages>(cameraQ, processQ, "unit_test", "unit_test_log.txt");
        };
    };


    TEST_F(TestProcessImage, ExtractImageInfo) {
        const auto elements = proc->extractImageInfo(imageForm01);        
        ASSERT_EQ(5, elements.size());
    }
   
}

// std::thread t1(&ProcessImages::start, &proc);
// t1.join();