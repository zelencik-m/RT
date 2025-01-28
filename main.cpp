#include <iostream>
#include <fstream>
#include <vector>

void savePPM(const std::string& filename, int width, int height, const std::vector<unsigned char>& image_data) {
    // Open the file for writing
    std::ofstream outFile(filename, std::ios::binary);
    
    if (!outFile) {
        std::cerr << "Error: Unable to open file for writing: " << filename << std::endl;
        return;
    }
    
    // Write the PPM header
    outFile << "P6\n";                // P6 indicates binary PPM format
    outFile << width << " " << height << "\n";  // Width and height
    outFile << "255\n";               // Max color value (255)
    
    // Write the pixel data (RGB values)
    outFile.write(reinterpret_cast<const char*>(image_data.data()), image_data.size());
    
    // Close the file
    outFile.close();
}

int main() {
    int width = 256;
    int height = 256;
    
    // Create a simple image: a gradient from black to white
    std::vector<unsigned char> image_data(width * height * 3);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = (y * width + x) * 3;
            unsigned char value = static_cast<unsigned char>((x + y) % 256);  // Simple gradient
            
            image_data[idx] = value;       // Red
            image_data[idx + 1] = value;   // Green
            image_data[idx + 2] = value;   // Blue
        }
    }

    // Save the image as a PPM file
    savePPM("output.ppm", width, height, image_data);
    
    std::cout << "Image saved as 'output.ppm'" << std::endl;

    return 0;
}
