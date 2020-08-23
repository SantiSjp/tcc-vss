#include <stdio.h>
#include <iostream>

#include <Vision/Control.h>
#include <Vision/CommandRobot.h>

int main(int argc, char **argv) {
    std::cout << "Starting" << std::endl;

    CommandRobot commandRobot;
    commandRobot.control.start();

    commandRobot.insertCommandInQueue(1,{5,10});
    
    return 0;
}