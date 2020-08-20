#include <stdio.h>
#include <iostream>

#include <Vision/Control.h>

int main(int argc, char **argv) {
    std::cout << "Starting" << std::endl;
    Control vision;
    vision.start();

    return 0;
}