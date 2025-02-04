#pragma once 

#include <glm/glm.hpp>

#include "image_loader.h"
#include "interval.h"

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

class imageTexture : public texture
{
public:
    imageTexture(std::string file) : texture(file){}

    glm::vec3 getColor(float u,float v, glm::vec3 pos) const override
    {
        // return glm::vec3(u,v,0.0f);
        if (texture.getHeigth() <= 0) return glm::vec3(0,1,1);
        // if(std::abs(u)<1)
        std::cout << int(u * (texture.getWidth()-1))<< " ";
        u = interval(0,1).clamp(u);
        v = 1.0 - interval(0,1).clamp(v);

        auto i = int(u * texture.getHeigth());
        auto j = int(v * texture.getWidth());
        return texture.getPixelColor(int(v * (texture.getWidth())),int(u * (texture.getWidth()-1)));
    }
public:
    ImageLoader texture;
};
