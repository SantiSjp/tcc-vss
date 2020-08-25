#ifndef PROCESSIMAGES_H
#define PROCESSIMAGES_H

#include <memory>

#include "PolyM/Queue.h"

class ProcessImages {
private:
    bool isRunning = false;
    int imageSeqNum = 0;
    
    PolyM::Queue& cameraQueue;

public:
    ProcessImages(PolyM::Queue& queue);

    void start();
    void stop();

};

#endif