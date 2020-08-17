#ifndef IMAGE_H
#define IMAGE_H

#include <vector>

//Placeholder class...

class Image {
private:
    std::vector<int> attributes;

public:
    Image (const std::vector<int> image) noexcept : attributes(image) {};
    std::vector<int> get(){return attributes;}
}

#endif