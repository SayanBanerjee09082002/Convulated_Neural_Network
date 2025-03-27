#include "layers.h"

std::vector<Image> Functional_Layer::perform_functional_operation(const Image input, const std::vector<int> &kernels, int convolution_stride, int pooling_size, int pooling_stride) {
    std::vector<Image> output = Convolution_Layer::perform_convolution(input, kernels, convolution_stride);
    
    for (size_t i = 0; i < output.size(); i++) 
        output[i] = Pooling_Layer::perform_pooling(output[i], pooling_size, pooling_stride);

    return output;
}