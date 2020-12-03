#ifndef COMMAND_H
#define COMMAND_H

#include <array>

#include "DataTypes.h"

class Command {
public:
    const vss::id robotId;
    const vss::position moveToPos;

    Command(const vss::id t_id, const vss::position movePos) 
        : robotId(t_id), moveToPos(movePos) {};

};

class CommandFactory {
public:
    static Command makeNew(const int id, const int posX, const int posY) {
        const vss::position pos({posX, posY}); 
        Command cmd(id, pos);
        return cmd;
    };
};

#endif