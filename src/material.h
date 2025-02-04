#pragma once

#include "hittable.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <random>
#include <memory>

glm::vec3 getRandVec();

class material
{
public:
    // virtual ~material() =default;

    virtual bool scatter(ray& in,hit_record& rec, glm::vec3& att, ray& out) const {return 0;}
    virtual glm::vec3 emitt(float u,float v, glm::vec3 pos)const
    {
        return glm::vec3(0.0f);
    }

};

class lambertian : public material
{
public:
    lambertian(glm::vec3 col) : tex(std::make_shared<solidColor>(col)){}
    lambertian(std::shared_ptr<texture> t) : tex(t){}

    bool scatter(ray& in,hit_record& rec, glm::vec3& att, ray& out) const override
    {
        glm::vec3 rand_vec = getRandVec();
        
        if(glm::dot(rand_vec,rec.normal)<=0.0f)
            rand_vec = -rand_vec;
        rand_vec = rec.normal + rand_vec;

        if (glm::all(glm::lessThan(glm::abs(rand_vec), glm::vec3(1e-6f))))
            rand_vec = rec.normal;
        
        out = ray(rec.p,rand_vec);
        att = tex->getColor(rec.u,rec.v,rec.p);
        return 1;
    }

private:
    std::shared_ptr<texture> tex;
};

class metal : public material
{
public:
    metal(glm::vec3 col,float fuzz) : albedo(col),fuzz(fuzz){}

    bool scatter(ray& in,hit_record& rec, glm::vec3& att, ray& out) const override
    {
        glm::vec3 ref_vec =  glm::reflect(in.getDirection(), rec.normal);
        ref_vec = glm::normalize(ref_vec) + (fuzz * getRandVec());
        out = ray(rec.p,ref_vec);
        att = albedo;
        return (glm::dot(out.getDirection(), rec.normal) > 0);
    }

private:
    glm::vec3 albedo;
    float fuzz;
};

class glass : public material
{
public:
    glass(float refraction) : refraction(refraction){}

    bool scatter(ray& in,hit_record& rec, glm::vec3& att, ray& out) const override
    {
        att = glm::vec3(1.0f);
        float ri = rec.front_face ? (1.0f / refraction) : refraction;

        glm::vec3 unit_dir = glm::normalize(in.getDirection());
        float cos_theta = std::fmin(glm::dot(-unit_dir,rec.normal),1.0f);
        float sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        bool cannon_ref = ri * sin_theta > 1.0f;
        glm::vec3 dir;

        if(cannon_ref || reflectance(cos_theta,ri) > std::rand())
            dir = glm::reflect(unit_dir,rec.normal);
        else 
            dir = glm::refract(unit_dir,rec.normal,ri);

        out = ray(rec.p,dir);
        return 1; 
    }

private:
    float refraction;

    static float reflectance(float cos,float ref_i)
    {
        auto r0 = (1-ref_i)/(1+ref_i);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1-cos),5);
    }
};

class diffuseLight : public material
{
public:
    diffuseLight(std::shared_ptr<texture> t) :tex(t){}
    diffuseLight(glm::vec3 col) : tex(std::make_shared<solidColor>(col)){}

    glm::vec3 emitt(float u,float v, glm::vec3 pos)const override
    {
        return tex->getColor(u,v,pos);
    }

private:
    std::shared_ptr<texture> tex;
};

glm::vec3 getRandVec()
{
    bool should_continue = 1;
    glm::vec3 rand_vec;
    while (should_continue)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

        rand_vec = glm::vec3(dist(gen), dist(gen), dist(gen));
        auto lenq = glm::length(rand_vec) * glm::length(rand_vec);
        if( lenq > 1e-160 && lenq <= 1.0 )
        {
            should_continue = 0;
            rand_vec = rand_vec / std::sqrt(lenq);
        }
    }
    return rand_vec;
}