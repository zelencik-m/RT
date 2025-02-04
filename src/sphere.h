#pragma once

#include <cmath>

#include "hittable.h"
#include "material.h"
#include "ray.h"


class sphere:public hittable
{
public:
    sphere(glm::vec3 c,float r, std::shared_ptr<material> m) : center(c),radius(std::fmax(0,r)), mat(m)
    {
        bbox = aabb(center-glm::vec3(radius), center+glm::vec3(radius));
    }
    
    bool hit( ray& r, float r_t_min, float r_t_max, hit_record& rec) const override
    {
        glm::vec3 oc = center- r.getOrigin();
        auto a = glm::dot(r.getDirection(),r.getDirection());
        auto h = dot(r.getDirection(),oc); 
        auto c = glm::dot(oc,oc) - (radius * radius); 
        auto dis = h*h - a * c;
        if (dis < 0)
            return false;
        
        float root = (h-std::sqrt(dis))/a;
        if(root < r_t_min || root >= r_t_max)
        {
            root = (h+std::sqrt(dis))/a;
            if(root <= r_t_min || root >= r_t_max)
                return false;
        }

        rec.p = r.at(root);
        rec.t = root;
        glm::vec3 normal_out = glm::normalize(rec.p - center);
        rec.set_normal(r,normal_out);
        getUV(normal_out,rec.u,rec.v);
        rec.mat = mat;
        return 1;
    }

    aabb boundingBox() const override {return bbox;}

private:
    glm::vec3 center;
    float radius;
    std::shared_ptr<material> mat;
    aabb bbox;

private:
    static void getUV(glm::vec3& p,float& u,float& v)
    {
        float theta = std::acos(-p.y);
        float phi = std::atan2(-p.z, p.x) + M_PI;

        u = phi / (2*M_PI);
        v = theta / M_PI;
    }

};
