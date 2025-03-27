#include "layers.h"

Image Pooling_Layer::perform_pooling(const Image &input, int pool_size, int stride) {
    int inputRows = input.pixels.size();
    int inputCols = input.pixels[0].size();

    int outputRows = (inputRows - pool_size) / stride + 1;
    int outputCols = (inputCols - pool_size) / stride + 1;
    std::vector<std::vector<uint8_t>> pooledPixels(outputRows, std::vector<uint8_t>(outputCols, 0));
    
    for (int i = 0; i < outputRows; ++i) {
        for (int j = 0; j < outputCols; ++j) {
            uint8_t maxVal = 0;
            for (int m = 0; m < pool_size; ++m) {
                for (int n = 0; n < pool_size; ++n) {
                    int row = i * stride + m;
                    int col = j * stride + n;
                    maxVal = std::max(maxVal, input.pixels[row][col]);
                }
            }
            pooledPixels[i][j] = maxVal;
        }
    }

    return Image(pooledPixels);
}