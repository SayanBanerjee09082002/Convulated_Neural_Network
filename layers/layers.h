#ifndef LAYERS_H
#define LAYERS_H

#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>
#include "../data/image.h"

class Kernel {
    public:
        std::vector<std::vector<float>> weights;
        std::string name;

        Kernel(const std::vector<std::vector<float>> &weights, const std::string &name = "");

        static Kernel sobelX();
        static Kernel sobelY();
        static Kernel sobelPlus45();
        static Kernel sobelMinus45();
        static Kernel laplacian90();
        static Kernel laplacian45();
        static Kernel emboss();
        static Kernel sharpen();
};

class Convolution_Layer {
    public:
        static std::vector<Image> perform_convolution(const Image &input, const std::vector<int> &kernels, int stride);
};

class Pooling_Layer {
    public:
        static Image perform_pooling(const Image &input, int pool_size, int stride);
};

class Functional_Layer {
    public:
        static std::vector<Image> perform_functional_operation(const Image input, const std::vector<int> &kernels, int convolution_stride = 1, int pooling_size = 3, int pooling_stride = 2);
};

class Fully_Connected_Neural_Network {
    public:
        
};

class Classification_Layer{

};

#endif