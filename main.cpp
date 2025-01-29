#include <iostream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext/vector_int2_sized.hpp>

#include "ray.h"

float hitSphere(glm::vec3 center, float radius, ray& r)
{
    glm::vec3 oc = center-r.getOrigin();
    auto a = glm::dot(r.getDirection(), r.getDirection());
    auto h = dot(r.getDirection(),oc); 
    auto c = glm::dot(oc, oc) - (radius * radius); 
    auto dis = h*h - a * c;
    if (dis < 0)
        return -1.0f;
    else
        return (h - std::sqrt(dis)) / a;
}

glm::i8vec3 getColor(ray& r)
{
    glm::vec3 retColor = glm::vec3(0.0f);

    float t = hitSphere(glm::vec3(0.0f,0.0f,-1.0f), 0.5,r);

    if(t != -1.0f)
    {
        glm::vec3 normal = glm::normalize(r.at(t) - glm::vec3(0.0f,0.0f,-1.0f));
        retColor = 0.5f * (normal + 1.0f);
    }
    else
    {
        float a = 0.5f * ((glm::normalize(r.getDirection()).y) + 1.0);
        retColor = (1.0f-a) * glm::vec3(1.0f) + a * glm::vec3(0.5f,0.7f,1.0f);
    }

    return glm::i8vec3(255 * retColor.r,255 * retColor.g,255 * retColor.b);
}

void savePPM(const std::string& filename, int width, int height, const std::vector<glm::i8vec3>& image_data);

int main() {
    float width = 800;
    float height = 400;
    
    auto aspect_ratio = width / height;

    float VP_width = 4.0;
    float VP_height = 2.0;
    float focal_length = 1.0;
    auto camera_center = glm::vec3(0.0f);

    auto VP_h = glm::vec3(VP_width,0.0f,0.0f);
    auto VP_v =  glm::vec3(0.0f,-VP_height,0.0f);
    auto pixel_delta_h = VP_h / width;
    auto pixel_delta_v = VP_v / height;

    auto VP_upper_left = camera_center - glm::vec3(0,0,focal_length) - VP_h/2.0f - VP_v/2.0f;
    auto pixel0 = VP_upper_left + 0.5f * (pixel_delta_h + pixel_delta_v);

    std::vector<glm::i8vec3> image_data(width * height*3);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            auto pixel_center = pixel0 + ((float)y * pixel_delta_v) + ((float)x * pixel_delta_h);
            auto ray_dir = pixel_center - camera_center;

            ray r(pixel_center,ray_dir);
            int idx = (y * width + x);
            image_data[idx] = getColor(r);

        }
    }

    savePPM("output.ppm", width, height, image_data);
    
    std::cout << "Image saved as 'output.ppm'" << std::endl;

    return 0;
}

void savePPM(const std::string& filename, int width, int height, const std::vector<glm::i8vec3>& image_data) {
    std::ofstream outFile(filename, std::ios::binary);
    
    if (!outFile) {
        std::cerr << "Error: Unable to open file for writing: " << filename << std::endl;
        return;
    }
    
    outFile << "P6\n";              
    outFile << width << " " << height << "\n";
    outFile << "255\n";

    outFile.write(reinterpret_cast<const char*>(image_data.data()), image_data.size());
    
    outFile.close();
}