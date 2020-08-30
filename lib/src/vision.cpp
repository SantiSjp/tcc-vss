#include <stdio.h>
#include <iostream>

#include <Vision/include/CommandInterface.h>

int main(int argc, char **argv) {
    std::cout << "Starting" << std::endl;
    {
        CommandInterface vision("/tmp/vision/capture");
        vss::color color1({0,1});
        vision.addNewRobot(1, color1, true);
        std::this_thread::sleep_for(std::chrono::milliseconds(8000));
        std::cout << "Ending" << std::endl;
    }
    
    return 0;
}