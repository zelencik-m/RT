#pragma once

#include <glm/glm.hpp>
#include <limits>

class interval
{
public:
    float min,max;

    interval() :min(std::numeric_limits<float>::infinity()), max(-std::numeric_limits<float>::infinity()) {}

    interval(float min, float max) : max(max), min(min){}

    interval(const interval& a, const interval& b)
    {
        min = std::min(a.min,b.min);
        max = std::min(a.max,b.max);
    }

    float size() const 
    {
        return max - min;
    }

    bool surrounds(float x) const
    {
        return min < x && x < max;
    }

    bool clamp(float x) const
    {
        if(x < min) return min;
        if(max < x) return max;
        return x;
    }

    interval expand(float delta) const
    {
        auto padding = delta/2;
        return interval(min-padding,max+padding);
    }
};