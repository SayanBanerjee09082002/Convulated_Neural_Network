#include <iostream>
#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

Image::Image(const std::string &file_path) 
    : width(0), height(0), channels(0) {
        unsigned char *data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);
        if (data) {
            pixels.resize(height, std::vector<uint8_t>(width, 0));
            if (channels == 1) {
                for (int i = 0; i < height; i++)
                    for (int j = 0; j < width; j++)
                        pixels[i][j] = data[i * width + j];
            }
            else {
                for (int i = 0; i < height; i++)
                    for (int j = 0; j < width; j++) {
                        int index = (i * width + j) * channels;
                        uint8_t grayscaled = static_cast<uint8_t>(
                            0.299 * data[index] +
                            0.587 * data[index + 1] +
                            0.114 * data[index + 2]
                        );
                        pixels[i][j] = grayscaled;
                    }
            }
            stbi_image_free(data);
        }
        else 
            std::cerr << "IMAGE LOADING ERROR: Could not load " << file_path << std::endl; 
    }

Image::~Image() {}