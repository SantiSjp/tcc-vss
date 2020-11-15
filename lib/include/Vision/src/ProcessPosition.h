#ifndef PROCESSPOSITION_H
#define PROCESSPOSITION_H

#include "DataTypes.h"
#include "Logger.h"
#include "Field.h"
#include "Command.h"

#include "PolyM/Queue.h"

#include <memory>

namespace vss {

class ProcessPosition {
private:
    bool isRunning = false;

    std::unique_ptr<Field> currentField;
    
    PolyM::Queue& m_processQueue;

    std::unique_ptr<Logger> m_logger;
    
    
    void configure();

public:
    ProcessPosition(PolyM::Queue& t_processQ,
                    const std::string& logName = "ProcessPosition", 
                    const std::string& logPath = "logs/vision_log.txt");
    
    ~ProcessPosition();
    
    void start();
    void stop();

    void calculateCurrentPosition(const std::vector<Element>& rawPosition);
    bool calculateFuturePosition(const Command& newCommand);

    Field& getCurrentField() const;
};

}

#endif