#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "hittable_list.h"
#include "ray.h"

class camera
{
public:
    camera(float w, float h, float VP_wi, float VP_he, glm::vec3 pos = glm::vec3(0.0f)) : width(w),height(h),VP_width(VP_wi),VP_height(VP_he),camera_pos(pos)
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
                auto ray_dir = pixel_center - camera_pos;

                ray r(pixel_center,ray_dir);
                int idx = (y * width + x);
                (*image_data)[idx] = getColor(r,world);

            }
        }
        return image_data;
    }

    float getWidth(){return width;}
    float getHeight(){return height;}

private:
    glm::i8vec3 getColor(ray& r, hittable_list world)
    {   
        glm::vec3 retColor = glm::vec3(0.0f);

        hit_record rec;
        if(world.hit(r,0,1000,rec))
        {
            retColor = 0.5f * (rec.normal + 1.0f);
        }
        else
        {
            float a = 0.5f * ((glm::normalize(r.getDirection()).y) + 1.0);
            retColor = (1.0f-a) * glm::vec3(1.0f) + a * glm::vec3(0.5f,0.7f,1.0f);
        }

        return glm::i8vec3(255 * retColor.r,255 * retColor.g,255 * retColor.b);
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
};