#include "image.h"

std::vector<std::vector<uint8_t>> apply_kernel(const std::vector<std::vector<uint8_t>> &pixels, const std::vector<std::vector<float>> &kernel, int stride) {
    int image_rows = pixels.size();
    int image_cols = pixels[0].size();
    int kernel_rows = kernel.size();
    int kernel_cols = kernel[0].size();

    int output_rows = (image_rows - kernel_rows) / stride + 1;
    int output_cols = (image_cols - kernel_cols) / stride + 1;

    std::vector<std::vector<uint8_t>> output(output_rows, std::vector<uint8_t>(output_cols, 0));

    for (int i = 0; i < output_rows; ++i) {
        for (int j = 0; j < output_cols; ++j) {
            float sum = 0.0f;
            for (int ki = 0; ki < kernel_rows; ++ki)
                for (int kj = 0; kj < kernel_cols; ++kj)
                    sum += pixels[i * stride + ki][j * stride + kj] * kernel[ki][kj];
            int result = static_cast<int>(std::round(sum));
            result = std::max(0, std::min(255, result));
            output[i][j] = static_cast<uint8_t>(result);
        }
    }
    
    return output;
}
