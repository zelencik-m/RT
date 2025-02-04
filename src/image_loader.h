#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <vector>
#include <glm/vec3.hpp>
#include <memory>
#include <iostream>


class ImageLoader {
public:
    std::vector<glm::vec3> image;
    int width, height;

public:
    ImageLoader(const std::string& filename) {
        int channels;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 3);
        std::cout << width << " "<< height << "\t";
        
        if (!data) {
            std::cerr << "Failed to load image: " << filename << std::endl;
            return;
        }

        image.reserve(width * height);
        for (int i = 0; i < width * height * 3; i += 3) {
            // std::cout << (float)data[i+1] << "\t";
            image.emplace_back(data[i] / 255.0f, data[i + 1] / 255.0f, data[i + 2] / 255.0f);
        }

        stbi_image_free(data);
    }

    glm::vec3 getPixelColor(int x, int y) const {
        if (x < 0 || x >= height) {
            std::cerr << "Pixel coordinates out of bounds. x"<< x << std::endl;
            return glm::vec3(0.0f);
        }
            if ( y < 0 || y >= width) {
            std::cerr << "Pixel coordinates out of bounds. y" << y<< std::endl;
            return glm::vec3(0.0f);
        }
        return image[x * width + y];
    }

    int getWidth() const {return width;}
    int getHeigth() const{return height;}
};