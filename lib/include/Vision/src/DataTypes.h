#ifndef DATATYPES_H
#define DATATYPES_H
#include <PolyM/Queue.h>
#include <PolyM/Msg.h>

#include <array>

#include "opencv2/core/utility.hpp"

namespace vss {
    using position = std::array<int, 2>;
    using lenght = std::array<int, 2>;
    using color = std::vector<int>;
    using id = int;
        
    class Element {
    public:
        Element(cv::Mat t_image, std::array<int,2> t_pos, id t_id = 0, bool t_ally = false, bool t_ball = false)
            : image(t_image), position(t_pos), ID(t_id), isAlly(t_ally), isBall(t_ball) {};
        
        const cv::Mat image;
        const std::array<int,2> position;
        
        bool isAlly = false;
        bool isBall = false;

        id ID;
    };

}


#endif