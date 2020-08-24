#include <stdio.h>
#include <iostream>

#include <Vision/Control.h>

int main(int argc, char **argv) {
    std::cout << "Starting" << std::endl;
    {
        Control vision;
        vision.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(15000));
        std::cout << "Ending" << std::endl;
    }
    
    return 0;
}