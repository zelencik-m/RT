#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <random>

#include <glm/glm.hpp>
#include <glm/ext/vector_int2_sized.hpp>

#include "hittable.h"
#include "quad.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

void savePPM(const std::string& filename, int width, int height, const std::shared_ptr<std::vector<glm::i8vec3> > image_data);
void setManySpheres(hittable_list& world);
void setChcekeredSpheres(hittable_list& world);
void setEarth(hittable_list& world);
void setQuads(hittable_list& world);
void setSimpleLight(hittable_list& world);
void setCornellBox(hittable_list& world);

// Function to generate a random float in the range [0,1]
float random_float() {
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    static std::mt19937 generator;
    return distribution(generator);
}

// Function to generate a random float in a given range
float random_float(float min, float max) {
    return min + (max - min) * random_float();
}

// Function to generate a random glm::vec3 in the range [0,1]
glm::vec3 random_vec3() {
    return glm::vec3(random_float(), random_float(), random_float());
}

// Function to generate a random glm::vec3 in a given range
glm::vec3 random_vec3(float min, float max) {
    return glm::vec3(random_float(min, max), random_float(min, max), random_float(min, max));
}

int main() {
    camera Camera(750.0,325.0,4.0,2.0,glm::vec3(0.0f,1.3f,5.0f)); 
    Camera.setBG(glm::vec3(0.0f));

    hittable_list world;
    setSimpleLight(world);

    std::shared_ptr<std::vector<glm::i8vec3> > image_data = Camera.render(world);

    savePPM("output.ppm", Camera.getWidth(), Camera.getHeight(), image_data);
    
    std::cout << "Image saved as 'output.ppm'" << std::endl;

    return 0;
}

void setManySpheres(hittable_list& world)
{
    auto checker = std::make_shared<checkerTexture>(glm::vec3(0.1f,0.9f,0.3f),2.0f);
    world.add(std::make_shared<sphere>(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f, std::make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_float();
            glm::vec3 center(a + 0.9f * random_float(), 0.2f, b + 0.9f * random_float());

            if (glm::length(center - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f) {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_vec3() * random_vec3();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    world.add(std::make_shared<sphere>(center, 0.2f, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random_vec3(0.5f, 1.0f);
                    auto fuzz = random_float(0.0f, 0.5f);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.add(std::make_shared<sphere>(center, 0.2f, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<glass>(1.5f);
                    world.add(std::make_shared<sphere>(center, 0.2f, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<glass>(1.5f);
    world.add(std::make_shared<sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, material1));

    auto material2 = std::make_shared<lambertian>(glm::vec3(0.4f, 0.2f, 0.1f));
    world.add(std::make_shared<sphere>(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, material2));

    auto material3 = std::make_shared<metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f);
    world.add(std::make_shared<sphere>(glm::vec3(4.0f, 1.0f, 0.0f), 1.0f, material3));
}

void setChcekeredSpheres(hittable_list& world)
{
    auto earth_texture = std::make_shared<imageTexture>("earthmap.jpg");
    auto earth_surface = std::make_shared<lambertian>(earth_texture);
    auto checker = std::make_shared<checkerTexture>(glm::vec3(.2, .3, .1),2.0f);

    world.add(std::make_shared<sphere>(glm::vec3(0,-10, 0), 10, earth_surface));
    world.add(std::make_shared<sphere>(glm::vec3(0, 10, 0), 10, std::make_shared<lambertian>(checker)));
}

void setEarth(hittable_list& world)
{
    auto earth_texture = std::make_shared<imageTexture>("earthmap.jpg");
    auto earth_surface = std::make_shared<lambertian>(earth_texture);
    world.add(std::make_shared<sphere>(glm::vec3(0,0, 0), 2, earth_surface));

}

void setQuads(hittable_list& world)
{
    // Materials
    auto left_red     = std::make_shared<lambertian>(glm::vec3(1.0, 0.2, 0.2));
    auto back_white   = std::make_shared<lambertian>(glm::vec3(0.9, 1.0, 0.85));
    auto right_blue   = std::make_shared<lambertian>(glm::vec3(0.2, 0.2, 1.0));
    auto upper_orange = std::make_shared<lambertian>(glm::vec3(1.0, 0.5, 0.0));
    auto lower_teal   = std::make_shared<lambertian>(glm::vec3(0.2, 0.8, 0.8));

    // Quads
    world.add(std::make_shared<quad>(glm::vec3(-2,-2, 0), glm::vec3(4, 0, 0), glm::vec3(0, 4, 0), back_white));
    world.add(std::make_shared<quad>(glm::vec3(-3,-2, 5), glm::vec3(0, 0,-4), glm::vec3(0, 4, 0), left_red));
    world.add(std::make_shared<quad>(glm::vec3( 3,-2, 1), glm::vec3(0, 0, 4), glm::vec3(0, 4, 0), right_blue));
    world.add(std::make_shared<quad>(glm::vec3(-2, 3, 1), glm::vec3(4, 0, 0), glm::vec3(0, 0, 4), upper_orange));
    world.add(std::make_shared<quad>(glm::vec3(-2,-3, 5), glm::vec3(4, 0, 0), glm::vec3(0, 0,-4), lower_teal));
}

void setSimpleLight(hittable_list& world)
{
    auto checker = std::make_shared<checkerTexture>(glm::vec3(.2, .3, .1),2.0f);
    auto difflight = std::make_shared<diffuseLight>(glm::vec3(1));
    auto left_red     = std::make_shared<lambertian>(glm::vec3(1.0, 0.2, 0.2));

    world.add(std::make_shared<sphere>(glm::vec3(0,-1000,0), 1000, std::make_shared<lambertian>(checker)));
    world.add(std::make_shared<sphere>(glm::vec3(0,2,0), 2, std::make_shared<lambertian>(checker)));
    world.add(std::make_shared<sphere>(glm::vec3(4,3,2), 2, difflight));
    world.add(std::make_shared<quad>(glm::vec3(-3,3,2), glm::vec3(2,0,0), glm::vec3(2,2,0), difflight));

}

void setCornellBox(hittable_list& world)
{
    auto red   = std::make_shared<lambertian>(glm::vec3(0.65f, 0.05f, 0.05f));
    auto white = std::make_shared<lambertian>(glm::vec3(0.73f, 0.73f, 0.73f));
    auto green = std::make_shared<lambertian>(glm::vec3(0.12f, 0.45f, 0.15f));
    auto light = std::make_shared<diffuseLight>(glm::vec3(15.0f, 15.0f, 15.0f));

    world.add(std::make_shared<quad>(glm::vec3(555.0f,0.0f,0.0f), glm::vec3(0.0f,555.0f,0.0f), glm::vec3(0.0f,0.0f,555.0f), green));
    world.add(std::make_shared<quad>(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,555.0f,0.0f), glm::vec3(0.0f,0.0f,555.0f), red));
    world.add(std::make_shared<quad>(glm::vec3(343.0f, 554.0f, 332.0f), glm::vec3(-130.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,-105.0f), light));
    world.add(std::make_shared<quad>(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(555.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,555.0f), white));
    world.add(std::make_shared<quad>(glm::vec3(555.0f,555.0f,555.0f), glm::vec3(-555.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,-555.0f), white));
    world.add(std::make_shared<quad>(glm::vec3(0.0f,0.0f,555.0f), glm::vec3(555.0f,0.0f,0.0f), glm::vec3(0.0f,555.0f,0.0f), white));
  
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