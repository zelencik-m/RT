#pragma once

#include "hittable.h"

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <random>

glm::vec3 getRandVec();

class material
{
public:
    virtual ~material() =default;

    virtual bool scatter(ray& in,hit_record& rec, glm::vec3& att, ray& out)
    const {return 0;}

};

class lambertian : public material
{
public:
    lambertian(glm::vec3 col) : albedo(col){}

    bool scatter(ray& in,hit_record& rec, glm::vec3& att, ray& out) const override
    {
        glm::vec3 rand_vec = getRandVec();
        
        if(glm::dot(rand_vec,rec.normal)<=0.0f)
            rand_vec = -rand_vec;
        rand_vec = rec.normal + rand_vec;

        if (glm::all(glm::lessThan(glm::abs(rand_vec), glm::vec3(1e-6f))))
            rand_vec = rec.normal;
        
        out = ray(rec.p,rand_vec);
        att = albedo;
        return 1;
    }

private:
    glm::vec3 albedo;
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