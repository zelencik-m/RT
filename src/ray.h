#pragma once

#include <glm/glm.hpp>

class ray
{
    public:
    ray(glm::vec3 origin, glm::vec3 direction):orig(origin),dir(direction){}

    const glm::vec3 getOrigin() {return orig;}
    const glm::vec3 getDirection() {return dir;}

    glm::vec3 at(float t)
    {
        return orig + (t * dir);
    }

    private:
    glm::vec3 orig;
    glm::vec3 dir;
};