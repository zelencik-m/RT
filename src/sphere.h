#pragma once

#include "hittable.h"
#include "material.h"
#include "ray.h"


class sphere:public hittable
{
public:
    sphere(glm::vec3 c,float r, std::shared_ptr<material> m) : center(c),radius(std::fmax(0,r)), mat(m){}
    
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
        rec.mat = mat;
        return 1;
    }

private:
    glm::vec3 center;
    float radius;
    std::shared_ptr<material> mat;

};
