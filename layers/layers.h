#ifndef LAYERS_H
#define LAYERS_H

#include <vector>
#include <string>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <random>
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

class Neural_Network {
    public:
        int input_size, hidden_size, output_size;
        std::vector<std::vector<float>> weights1;
        std::vector<float> bias1;  
        std::vector<std::vector<float>> weights2;
        std::vector<float> bias2;
        float learning_rate;

        Neural_Network(int input_size, int hidden_size, int output_size, float learning_rate = 0.0001);
        ~Neural_Network();
        void train(const std::vector<float>& input, int label);
        int predict(const std::vector<float>& input);
    
    private:
        std::vector<float> forward_pass(const std::vector<float>& input, std::vector<float>& hidden_out);
        void backward_pass(const std::vector<float>& input, const std::vector<float>& hidden, const std::vector<float>& output, int target);
};

#endif