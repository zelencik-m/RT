#include <iostream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext/vector_int2_sized.hpp>

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

void savePPM(const std::string& filename, int width, int height, const std::shared_ptr<std::vector<glm::i8vec3> > image_data);

int main() {
    camera Camera(800.0,400.0,4.0,2.0); 

    hittable_list world;
    world.add(std::make_shared<sphere>(glm::vec3(0.0f,0.0f,-1.0f),0.5));
    world.add(std::make_shared<sphere>(glm::vec3(0.0f,-100.0f,-1.0f),100));

    std::shared_ptr<std::vector<glm::i8vec3> > image_data = Camera.render(world);

    savePPM("output.ppm", Camera.getWidth(), Camera.getHeight(), image_data);
    
    std::cout << "Image saved as 'output.ppm'" << std::endl;

    return 0;
}

void savePPM(const std::string& filename, int width, int height, const std::shared_ptr<std::vector<glm::i8vec3> > image_data) {
    std::ofstream outFile(filename, std::ios::binary);
    
    if (!outFile) {
        std::cerr << "Error: Unable to open file for writing: " << filename << std::endl;
        return;
    }
    
    outFile << "P6\n";              
    outFile << width << " " << height << "\n";
    outFile << "255\n";

    outFile.write(reinterpret_cast<const char*>((*image_data).data()), (*image_data).size());
    
    outFile.close();
}