#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "data/image.h"
#include "layers/layers.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

namespace fs = std::filesystem;

void saveGrayscaleImage(const Image &img, const std::string &outputPath) {
    std::vector<uint8_t> imageData(img.width * img.height);
    for (int i = 0; i < img.height; ++i)
        for (int j = 0; j < img.width; ++j)
            imageData[i * img.width + j] = img.pixels[i][j];

    if (!stbi_write_png(outputPath.c_str(), img.width, img.height, 1, imageData.data(), img.width))
        std::cerr << "ERROR: Could not save image to " << outputPath << std::endl;
}

std::vector<std::string> listImageFiles(const std::string &folderPath) {
    std::vector<std::string> imageFiles;
    for (const auto &entry : fs::directory_iterator(folderPath))
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp" || ext == ".tga")
                imageFiles.push_back(entry.path().string());
        }
    return imageFiles;
}

int main() {
    std::string folderPath = "images";
    std::vector<std::string> imageFiles = listImageFiles(folderPath);
    if (imageFiles.empty()) {
        std::cerr << "No image files found in folder: " << folderPath << std::endl;
        return 1;
    }

    std::vector<Image> images;
    for (const auto &filePath : imageFiles) {
        std::cout << "Loading image: " << filePath << std::endl;
        images.emplace_back(filePath);
    }

    // Define the kernels
    std::vector<int> kernelIDs1 = {0, 1, 2, 3, 4, 5};
    std::vector<int> kernelIDs2 = {6, 7};

    // Define parameters for convolution and pooling
    int convolutionStride = 1;
    int poolingSize = 3;
    int poolingStride = 2;

    // First functional operation
    std::vector<Image> firstPassImages;
    for (const auto &img : images) {
        std::vector<Image> processed = Functional_Layer::perform_functional_operation(img, kernelIDs1, convolutionStride, poolingSize, poolingStride);
        firstPassImages.insert(firstPassImages.end(), processed.begin(), processed.end());
    }

    // Second functional operation on the results of the first pass
    std::vector<Image> finalImages;
    for (const auto &img : firstPassImages) {
        std::vector<Image> processed = Functional_Layer::perform_functional_operation(img, kernelIDs2, convolutionStride, poolingSize, poolingStride);
        finalImages.insert(finalImages.end(), processed.begin(), processed.end());
    }

    std::cout << "\nProcessing complete. Saving images...\n";

    int count = 1;
    for (const auto &img : finalImages) {
        std::string outputPath = "op/func_image_" + std::to_string(count++) + ".png";
        saveGrayscaleImage(img, outputPath);
    }

    return 0;
}
