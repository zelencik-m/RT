#pragma once 

#include <glm/glm.hpp>

class texture
{
public:
    virtual glm::vec3 getColor(float u,float v, glm::vec3 pos) const = 0;
};

class solidColor : public texture
{
public:
    solidColor(glm::vec3 col) : albedo(col){}

    glm::vec3 getColor(float u,float v, glm::vec3 pos) const override
    {
        return albedo;
    }
private:
    glm::vec3 albedo;
};

class checkerTexture : public texture
{
public:
    checkerTexture(glm::vec3 col,float s) : albedo(col),scale(s){}

    glm::vec3 getColor(float u,float v, glm::vec3 pos) const override
    {
        auto x = int(std::floor(scale * pos.x));
        auto y = int(std::floor(scale * pos.y));
        auto z = int(std::floor(scale * pos.z));

        bool isEven = (x+y+z)%2 == 0;

        return isEven ? albedo : glm::vec3(1.0f);
    }
private:
    float scale;
    glm::vec3 albedo;
};