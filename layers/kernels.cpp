#include "layers.h"

Kernel::Kernel(const std::vector<std::vector<float>> &weights, const std::string &name)
    : weights(weights), name(name) { }
    
// Sobel filters for edge detection
Kernel Kernel::sobelX() {
    std::vector<std::vector<float>> weights = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    return Kernel(weights, "sobel_x");
}

Kernel Kernel::sobelY() {
    std::vector<std::vector<float>> weights = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    return Kernel(weights, "sobel_y");
}

Kernel Kernel::sobelPlus45() {
    std::vector<std::vector<float>> weights = {
        {2, 1, 0},
        {1, 0, -1},
        {0, -1, -2}
    };
    return Kernel(weights, "sobel_+45");
}

Kernel Kernel::sobelMinus45() {
    std::vector<std::vector<float>> weights = {
        {0, 1, 2},
        {-1, 0, 1},
        {-2, -1, 0}
    };
    return Kernel(weights, "sobel_-45");
}

// Laplacian filter (detects edges by second-order derivatives)
Kernel Kernel::laplacian90() {
    std::vector<std::vector<float>> weights = {
        { 0, -1,  0},
        {-1,  4, -1},
        { 0, -1,  0}
    };
    return Kernel(weights, "laplacian_90");
}

Kernel Kernel::laplacian45() {
    std::vector<std::vector<float>> weights = {
        { -1, -1,  -1},
        {-1,  8, -1},
        { -1, -1,  -1}
    };
    return Kernel(weights, "laplacian_45");
}


// Sharpen Edges
Kernel Kernel::emboss() {
    std::vector<std::vector<float>> weights = {
        {-2, -1,  0},
        {-1,  1,  1},
        { 0,  1,  2}
    };
    return Kernel(weights, "emboss");
}

Kernel Kernel::sharpen() {
    std::vector<std::vector<float>> weights = {
        { 0, -1,  0},
        {-1,  5, -1},
        { 0, -1,  0}
    };
    return Kernel(weights, "sharpen");
}