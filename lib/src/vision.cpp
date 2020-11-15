#include <stdio.h>
#include <iostream>

#include <Vision/include/CommandInterface.h>

bool isRunning = true;
int selection = -1;
int robotIDs = 0;
const vss::color allyColor({1,2});
const vss::color enemyColor({3,4});

int main(int argc, char **argv) {
    CommandInterface vision("/tmp/vision/capture");
    system("clear");    
    do {
        std::cout << "---- Vision v1 ----"      << std::endl; 
        std::cout << "Select one action:"       << std::endl;
        std::cout << "1. Add Ally Robot"        << std::endl;
        std::cout << "2. Add Enemy Robot"       << std::endl; 
        std::cout << "3. View Robot Position"   << std::endl;
        std::cout << "9. Exit"                  << std::endl;
        std::cout << std::endl;
        std::cout << ": ";
        std::cin >> selection;

        switch (selection) {
        case 1:
            vision.addNewRobot(++robotIDs, allyColor, true);
            std::cout << "Adding robot id [" << robotIDs << "]" << std::endl;
            break;
        case 2:
            vision.addNewRobot(++robotIDs, enemyColor, false);
            std::cout << "Adding robot id [" << robotIDs << "]" << std::endl;
            break;
        case 3:
            std::cout << "ally pos:" << vision.getPositions()[0] << "," << vision.getPositions()[1] << std::endl;
            break;
        case 9:
            isRunning = false;
            break;
        default:
            std::cout << "Not a valid command..." << std::endl;
            break;
        }
        
        std::cout << "Press any key to continue.";
        std::cin.ignore();
        std::cin.get();
        system("clear");

    } while(isRunning);

    std::cout << "Ending..." << std::endl;
    system("clear");
    return 0;
}