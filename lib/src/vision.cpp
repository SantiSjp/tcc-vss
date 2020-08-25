#include <stdio.h>
#include <iostream>

#include <Vision/Control.h>

int main(int argc, char **argv) {
    std::cout << "Starting" << std::endl;
    {
        Control vision("/tmp/vision/capture");
        std::this_thread::sleep_for(std::chrono::milliseconds(8000));
        std::cout << "Ending" << std::endl;
    }
    
    return 0;
}