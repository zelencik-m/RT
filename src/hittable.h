#pragma once

#include "ray.h"

class hit_record
{
    public:
    glm::vec3 p;
    glm::vec3 normal;
    float t;
    bool front_face;

    void set_normal(ray& r, glm::vec3& normal_out)
    {
        front_face = dot(r.getDirection(), normal_out) < 0;
        normal = front_face ? normal_out : -normal_out;
    }
};

class hittable
{
    public:
    virtual ~hittable() = default;

    virtual bool hit( ray& r, float r_t_min, float r_t_max, hit_record& rec) const = 0;
};