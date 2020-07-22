#include <stdio.h>
#include <iostream>

#include <Utils.h>

int main(int argc, char **argv) {
    const std::string teste {"teste"};
    const std::string uppercase {Utils::uppercase(teste)};

    std::cout << "result: " << uppercase << std::endl;
    return 0;
}

