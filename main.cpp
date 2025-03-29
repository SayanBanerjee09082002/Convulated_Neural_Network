#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <limits.h>
#include "data/image.h"
#include "layers/layers.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

namespace fs = std::filesystem;

// Structure to hold image file path and its associated label.
struct Sample {
    std::string imagePath;
    int label;
};

void saveGrayscaleImage(const Image &img, const std::string &outputPath) {
    std::vector<uint8_t> imageData(img.width * img.height);
    for (int i = 0; i < img.height; ++i)
        for (int j = 0; j < img.width; ++j)
            imageData[i * img.width + j] = img.pixels[i][j];

    if (!stbi_write_png(outputPath.c_str(), img.width, img.height, 1, imageData.data(), img.width))
        std::cerr << "ERROR: Could not save image to " << outputPath << "\n";
}

int main() {
    std::string trainingRoot = "nums/train"; 
    std::string testRoot = "nums/test";
    
    // Convolution and pooling parameters.
    std::vector<int> kernelIDs1 = {0, 1, 2, 3, 4, 5};
    int convolutionStride = 1;
    int poolingSize = 3;
    int poolingStride = 2;
    int numClasses = 10;
    int max_samples = 1000; 
    int num_epochs = 100;
    
    // -------------------------
    // Gather training samples into a vector (using the label attribute in Image).
    // -------------------------
    std::vector<Sample> trainingSamples;
    for (const auto &classEntry : fs::directory_iterator(trainingRoot)) {
        if (classEntry.is_directory()) {
            // Folder name should be the label.
            std::string className = classEntry.path().filename().string();
            int label = std::stoi(className);
            int sampleCount = 0;
            for (const auto &fileEntry : fs::directory_iterator(classEntry.path())) {
                if (sampleCount >= max_samples)
                    break;
                if (fileEntry.is_regular_file()) {
                    std::string ext = fileEntry.path().extension().string();
                    if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" ||
                        ext == ".bmp" || ext == ".tga") {
                        trainingSamples.push_back({fileEntry.path().string(), label});
                        sampleCount++;
                    }
                }
            }
        }
    }
    
    // -------------------------
    // Determine flattened size from a sample image.
    // -------------------------
    int flattenedSize = 0;
    bool foundSample = false;
    if (!trainingSamples.empty()) {
        Image sampleImg(trainingSamples[0].imagePath, trainingSamples[0].label);
        std::vector<Image> finalImages = Functional_Layer::perform_functional_operation(
            sampleImg, kernelIDs1, convolutionStride, poolingSize, poolingStride);
        if (!finalImages.empty()) {
            flattenedSize = finalImages[0].width * finalImages[0].height;
            foundSample = true;
            std::cout << "Determined flattened size: " << flattenedSize << "\n";
        }
    }
    if (!foundSample) {
        std::cerr << "No sample image found to determine flattened size. Exiting.\n";
        return 1;
    }
    
    Neural_Network nn(flattenedSize, 128, numClasses, 0.000001);
    
    std::cout << "Starting training for " << num_epochs << " epochs...\n";
    for (int epoch = 1; epoch <= num_epochs; epoch++) {
        std::cout << "Epoch " << epoch << ":\n";

        // Shuffle the training samples.
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(trainingSamples.begin(), trainingSamples.end(), g);
        
        // Iterate over all shuffled training samples.
        for (const auto &sample : trainingSamples) {
            Image img(sample.imagePath, sample.label);
            
            // Process the image through the functional layer.
            std::vector<Image> finalImages = Functional_Layer::perform_functional_operation(
                img, kernelIDs1, convolutionStride, poolingSize, poolingStride);
            
            // Flatten each final image and train the network.
            for (const auto &finalImg : finalImages) {
                std::vector<float> flattened;
                flattened.reserve(finalImg.width * finalImg.height);
                for (int i = 0; i < finalImg.height; i++) {
                    for (int j = 0; j < finalImg.width; j++) {
                        flattened.push_back(finalImg.pixels[i][j] / 255.0f);
                    }
                }
                nn.train(flattened, sample.label);
            }
        }
        
        // -------------------------
        // Testing Phase after each epoch
        // -------------------------
        std::cout << "Testing after epoch " << epoch << ":\n";
        for (const auto &fileEntry : fs::directory_iterator(testRoot)) {
            if (fileEntry.is_regular_file()) {
                std::string ext = fileEntry.path().extension().string();
                if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" ||
                    ext == ".bmp" || ext == ".tga") {
                    // label is not needed so pass dummy value (-1).
                    Image img(fileEntry.path().string(), -1);
                    
                    std::vector<Image> finalImages = Functional_Layer::perform_functional_operation(
                        img, kernelIDs1, convolutionStride, poolingSize, poolingStride);
                    
                    // majority vote
                    std::unordered_map<int, int> voteCount;
                    for (const auto &finalImg : finalImages) {
                        std::vector<float> flattened;
                        flattened.reserve(finalImg.width * finalImg.height);
                        for (int i = 0; i < finalImg.height; i++) {
                            for (int j = 0; j < finalImg.width; j++) {
                                flattened.push_back(finalImg.pixels[i][j] / 255.0f);
                            }
                        }
                        int predictedLabel = nn.predict(flattened);
                        voteCount[predictedLabel]++;
                    }
                    
                    // Determine the majority vote prediction.
                    int finalPrediction = -1;
                    int maxVotes = 0;
                    for (const auto &vote : voteCount) {
                        if (vote.second > maxVotes) {
                            maxVotes = vote.second;
                            finalPrediction = vote.first;
                        }
                    }
                    
                    std::cout << "Predicted class for " 
                              << fileEntry.path().filename().string() 
                              << " is: " << finalPrediction << "\n";
                }
            }
        }
        std::cout << "-------------------------------------------\n";
    }
    
    std::cout << "Training and testing complete.\n";
    
    return 0;
}
