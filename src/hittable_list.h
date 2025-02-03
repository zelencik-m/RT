#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class hittable_list
{
public:
    std::vector<std::shared_ptr<hittable> > objects;

    hittable_list(){}
    hittable_list(std::shared_ptr<hittable> object) {add(object);}

    void clear(){objects.clear();}
    void add(std::shared_ptr<hittable> object)
    {
        objects.push_back(object);
        bbox = aabb(bbox, object->boundingBox());
    }

    bool hit(ray& r, float r_t_min, float r_t_max, hit_record& rec)
    {
        hit_record temp_rec;
        bool was_hit = 0;
        auto closest = r_t_max;

        for(const auto& object: objects)
        {
            if(object->hit(r,r_t_min,closest,temp_rec))
            {
                was_hit = 1;
                closest = temp_rec.t;
                rec=temp_rec;
            }
        }
        return was_hit;
    }

private:
    aabb bbox;
};