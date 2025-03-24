#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "data/image.h"

namespace fs = std::filesystem;

// Helper function to display pixel values of an image.
// For large images, consider limiting the output (here we print all pixels).
void displayImagePixels(const Image &img) {
    std::cout << "Displaying Image (" << img.width << " x " << img.height << "):\n";
    for (int i = 0; i < img.height; ++i) {
        for (int j = 0; j < img.width; ++j) {
            // Cast to int for easier reading.
            std::cout << static_cast<int>(img.pixels[i][j]) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

std::vector<std::string> listImageFiles(const std::string &folderPath) {
    std::vector<std::string> imageFiles;
    for (const auto &entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp" || ext == ".tga") 
                imageFiles.push_back(entry.path().string());
        }
    }
    return imageFiles;
}

int main() {
    // Change this to the folder where your images are stored.
    std::string folderPath = "images";
    
    // Get list of image files in the folder.
    std::vector<std::string> imageFiles = listImageFiles(folderPath);
    
    if (imageFiles.empty()) {
        std::cerr << "No image files found in folder: " << folderPath << std::endl;
        return 1;
    }
    
    // Create a vector to store the images.
    std::vector<Image> images;
    for (const auto &filePath : imageFiles) {
        std::cout << "Loading image: " << filePath << std::endl;
        images.emplace_back(filePath);
    }
    
    // Iterate over each image and display its pixels.
    for (const auto &img : images) {
        displayImagePixels(img);
        std::cout << "--------------------------------\n";
    }
    
    return 0;
}