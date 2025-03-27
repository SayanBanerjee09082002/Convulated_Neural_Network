#include "layers.h"

Kernel get_kernel(int name) {
    switch (name) {
        case 0:
            return Kernel::sobelX();
        case 1: 
            return Kernel::sobelY();
        case 2: 
            return Kernel::sobelPlus45();
        case 3:
            return Kernel::sobelMinus45();
        case 4:
            return Kernel::laplacian90();
        case 5:
            return Kernel::laplacian45();
        case 6: 
            return Kernel::emboss();
        case 7:
            return Kernel::sharpen();
        default:
            return Kernel::sobelX();
    }
}

std::vector<Image> Convolution_Layer::perform_convolution(const Image &input, const std::vector<int> &kernels, int stride) {
    std::vector<Image> output;
    for (int kernel : kernels) {
        Kernel k = get_kernel(kernel);
        std::vector<std::vector<uint8_t>> result = apply_kernel(input.pixels, k.weights, stride);
        Image img = Image(result);
        output.push_back(img);
    }
    return output;
}