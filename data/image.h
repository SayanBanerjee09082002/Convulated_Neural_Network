#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <cstdint>
#include <string>
#include <cmath> 

class Image {
    public:
        int width;
        int height;
        int channels;
        int label;
        std::vector<std::vector<uint8_t>> pixels;
        
        Image(const std::string &file_path, int label);
        Image(const std::vector<std::vector<uint8_t>> &result, int label);
        ~Image();
};

std::vector<std::vector<uint8_t>> apply_kernel(const std::vector<std::vector<uint8_t>> &pixels, const std::vector<std::vector<float>> &kernel, int stride);

#endif