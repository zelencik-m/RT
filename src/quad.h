#pragma once

#include "hittable.h"

class quad : public hittable
{
public:
    quad(glm::vec3 q, glm::vec3 u,glm::vec3 v,std::shared_ptr<material>m):Q(q),u(u),v(v),mat(m)
    {
        auto n = glm::cross(u,v);
        normal = glm::normalize(n);
        D = glm::dot(normal,Q);
        w = n/glm::dot(n,n);

        setBoundingBox();
    }
    void setBoundingBox()
    {
        auto bbox_1 = aabb(Q,Q+u+v);
        auto bbox_2 = aabb(Q+u,Q+v);

        bbox = aabb(bbox_1,bbox_2);
    }

    virtual aabb boundingBox() const override {return bbox;}

    virtual bool hit( ray& r, float r_t_min, float r_t_max, hit_record& rec) const override
    {
        auto denom = glm::dot(normal,r.getDirection());

        if(std::fabs(denom)<1e-8)
            return 0;

        auto t = (D-dot(normal,r.getOrigin()))/ denom;
        if(t < r_t_min || t > r_t_max)
            return 0;

        glm::vec3 planar= r.at(t) - Q;
        auto alpha = glm::dot(w,glm::cross(planar,v));
        auto beta = glm::dot(w,glm::cross(u,planar));
        if(!isInside(alpha,beta,rec))
            return 0;

        rec.t = t;
        rec.p = r.at(t);
        rec.mat = mat;
        glm::vec3 normal_out = normal;
        rec.set_normal(r,normal_out);
        return 1;
    }

    bool isInside(float a,float b,hit_record& rec)const
    {
        interval unit_i = interval(0,1);

        if(!unit_i.surrounds(a) || !unit_i.surrounds(b))
            return 0;

        rec.u = a;
        rec.v = b;
        return 1;
    }


private:
    glm::vec3 Q;
    glm::vec3 u,v;
    glm::vec3 w;
    std::shared_ptr<material> mat;
    aabb bbox;

    glm::vec3 normal;
    float D;

};