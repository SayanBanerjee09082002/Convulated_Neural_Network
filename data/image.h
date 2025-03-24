#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <cstdint>
#include <string>

class Image {
    public:
        int width;
        int height;
        int channels;
        std::vector<std::vector<uint8_t>> pixels;
        
        Image(const std::string &file_path);
        ~Image();
};

#endif