#pragma once

#include "ray.h"
#include "aabb.h"

#include<memory>

class material;

class hit_record
{
public:
    glm::vec3 p;
    glm::vec3 normal;
    std::shared_ptr<material> mat;
    float t;
    float u;
    float v;
    bool front_face;

    void set_normal(ray& r, glm::vec3& normal_out)
    {
        front_face = glm::dot(glm::normalize(r.getDirection()), normal_out) < 0.0f;
        normal = front_face ? normal_out : -normal_out;
    }
};

class hittable
{
public:
    // virtual ~hittable() = default;

    virtual bool hit( ray& r, float r_t_min, float r_t_max, hit_record& rec) const = 0;

    virtual aabb boundingBox() const = 0;
};