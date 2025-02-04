#pragma once

#include <glm/glm.hpp>
#include <limits>

#include "ray.h"
#include "interval.h"


class aabb 
{
public:
    interval x,y,z;

    aabb(){}

    aabb(const interval& x,const interval&y,const interval&z) :x(x),y(y),z(z) 
    {
        padToMins();
    }

    aabb(glm::vec3 a, glm::vec3 b)
    {
        x = a.x <= b.x ? interval(a.x,b.x) : interval(b.x,a.x);
        y = a.y <= b.y ? interval(a.y,b.y) : interval(b.y,a.y);
        z = a.z <= b.z ? interval(a.z,b.z) : interval(b.z,a.z);

        padToMins();

    }

    aabb(const aabb& b0,const aabb& b1)
    {
        x = interval(b0.x,b1.x);
        y = interval(b0.y,b1.y);
        z = interval(b0.z,b1.z);
    }

    const interval& axInt(int n)
    {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    bool hit(ray& r, interval r_t)
    {
        for(int ax = 0; ax < 3;ax++)
        {
            const float a_div = 1.0f / r.getDirection()[ax];

            auto t0 = (axInt(ax).min - r.getOrigin()[ax]) * a_div;
            auto t1 = (axInt(ax).max - r.getOrigin()[ax]) * a_div;

            if(t0 < t1)
            {
                if(t0 > r_t.min ) r_t.min = t0;
                if(t1 < r_t.max ) r_t.max = t1;
            }
            else
            {
                if(t1 > r_t.min ) r_t.min = t1;
                if(t0 < r_t.max ) r_t.max = t0;
            }

            if(r_t.max <= r_t.min)
                return 0;
        }
        return 1;
    }

    int longestAxis()
    {
        if(x.size() > y.size())
            return x.size() > y.size() ? 0 : 2;
        else
            return z.size() > y.size() ? 1 : 2;

    }

private:
    void padToMins()
    {
        float delta = 0.00001;
        if(x.size()< delta)x.expand(delta);
        if(y.size()< delta)y.expand(delta);
        if(z.size()< delta)z.expand(delta);
    }
};