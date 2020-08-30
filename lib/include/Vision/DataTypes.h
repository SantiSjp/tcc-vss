#ifndef DATATYPES_H
#define DATATYPES_H
#include <PolyM/Queue.h>
#include <PolyM/Msg.h>

#include <array>

namespace vss {
    using position = std::array<int, 2>;
    using color = std::vector<int>;
    using id = int;

}


#endif