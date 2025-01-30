#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <cstdlib>
#include <random>

#include "hittable_list.h"
#include "ray.h"

class camera
{
public:
    camera(float w, float h, float VP_wi, float VP_he, glm::vec3 pos = glm::vec3(0.0f),int samples = 8) : width(w),height(h),VP_width(VP_wi),VP_height(VP_he),camera_pos(pos), sampels_per_pixel(samples)
    {   
        auto aspect_ratio = width / height;

        float VP_width = 4.0;
        float VP_height = 2.0;
        float focal_length = 1.0;
        auto camera_center = glm::vec3(0.0f);

        auto VP_h = glm::vec3(VP_width,0.0f,0.0f);
        auto VP_v =  glm::vec3(0.0f,-VP_height,0.0f);
        pixel_delta_h = VP_h / width;
        pixel_delta_v = VP_v / height;

        auto VP_upper_left = camera_center - glm::vec3(0,0,focal_length) - VP_h/2.0f - VP_v/2.0f;
        pixel0 = VP_upper_left + 0.5f * (pixel_delta_h + pixel_delta_v);
    }

    std::shared_ptr<std::vector<glm::i8vec3> > render(hittable_list& world)
    {
        std::shared_ptr<std::vector<glm::i8vec3> > image_data = std::make_shared<std::vector<glm::i8vec3> >(width * height*3);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {

                auto pixel_center = pixel0 + ((float)y * pixel_delta_v) + ((float)x * pixel_delta_h);
                auto offset_v = ((pixel_delta_v / 4.0f) * (((std::rand()%2000)-1000)/1000.0f));
                auto offset_h = ((pixel_delta_h / 4.0f) * (((std::rand()%2000)-1000)/1000.0f));

                auto ray_dir = pixel_center - camera_pos;
                glm::vec3 AA_pos = pixel_center + offset_v + offset_h;
                glm::vec3 AA_dir = AA_pos - camera_pos;
                ray r(AA_pos,AA_dir);
                glm::vec3 pixelCol = getColor(r,world,0);
                for (int i = 0; i < sampels_per_pixel-1; i++)
                {           
                    offset_v = ((pixel_delta_v / 4.0f) * (((std::rand()%2000)-1000)/1000.0f));
                    offset_h = ((pixel_delta_h / 4.0f) * (((std::rand()%2000)-1000)/1000.0f));
                    auto ray_dir = pixel_center - camera_pos;
                    glm::vec3 AA_pos = pixel_center + offset_v + offset_h;
                    glm::vec3 AA_dir = AA_pos - camera_pos;
                    ray r(AA_pos,AA_dir);
                    pixelCol += getColor(r,world,0);
                    pixelCol = pixelCol / 2.0f;
                }
                int idx = (y * width + x);
                (*image_data)[idx] = glm::i8vec3(255 * pixelCol.r,255 * pixelCol.g,255 * pixelCol.b);

            }
        }
        return image_data;
    }

    float getWidth(){return width;}
    float getHeight(){return height;}

private:
    glm::vec3 getColor(ray& r, hittable_list& world,int bouce_count)
    {   
        if(bouce_count > max_bouce_count)
            return glm::vec3(0.0f);

        hit_record rec;
        if(world.hit(r,0,10000000,rec))
        {
            // bool should_continue = 1;
            // glm::vec3 rand_vec;
            // while (should_continue)
            // {
            //     static std::random_device rd;
            //     static std::mt19937 gen(rd());
            //     std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

            //     rand_vec = glm::vec3(dist(gen), dist(gen), dist(gen));
            //     auto lenq = glm::length(rand_vec) * glm::length(rand_vec);
            //     if( 1e-160 < lenq <= 1.0)
            //     {
            //         should_continue = 0;
            //         rand_vec = rand_vec / std::sqrt(lenq);
            //     }
            // }
            
            // if(glm::dot(rand_vec,rec.normal)<=0.0f)
            //     rand_vec = -rand_vec;

            // ray ra(rec.p,rand_vec);
            // return 0.5f * getColor(ra, world,bouce_count+1);
            return 0.5f * (rec.normal + 1.0f);

        }
        float a = 0.5f * ((glm::normalize(r.getDirection()).y) + 1.0);
        return (1.0f-a) * glm::vec3(1.0f) + a * glm::vec3(0.5f,0.7f,1.0f);
    }

private:
    float width;
    float height;
    float VP_width;
    float VP_height;
    glm::vec3 pixel0;
    glm::vec3 pixel_delta_h;
    glm::vec3 pixel_delta_v;

    glm::vec3 camera_pos;
    int sampels_per_pixel;

    int max_bouce_count = 10;
};