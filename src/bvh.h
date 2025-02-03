#pragma once

#include <memory>
#include <vector>
#include <random>
#include <algorithm>

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

class bvh_node : public hittable
{
public:
    bvh_node(hittable_list list) : bvh_node(list.objects,0,list.objects.size())
    {

    }

    bvh_node(std::vector<std::shared_ptr<hittable> >& objs, size_t start, size_t end)
    {
        bbox = aabb(interval(),    interval(),    interval());

        for(size_t obj_i = start; obj_i < end;obj_i++)
            bbox = aabb (bbox,objs[obj_i]->boundingBox());

        int axis = bbox.longestAxis();

        auto comparator = (axis == 0) ? boxXCompare
                        : (axis == 1) ? boxYCompare
                                      : boxZCompare;

        size_t obj_span = end-start;

        if(obj_span == 1)
        {
            left = right = objs[start];
        }
        else if(obj_span == 2)
        {
            left = objs[start];
            right = objs[start+1];
        }
        else
        {
            std::sort(std::begin(objs) + start,std::begin(objs)+end,comparator);

            auto mid = start + obj_span/2;
            left = std::make_shared<bvh_node>(objs,start,mid);
            right= std::make_shared<bvh_node>(objs,mid,end);
        }

        bbox = aabb(left->boundingBox(), right->boundingBox());
    }

    bool hit(ray& r, float r_t_min, float r_t_max, hit_record& rec) const override
    {
        interval intr(r_t_min,r_t_max);
        if(!bbox.hit(r,intr)) return 0;

        bool hit_l = left->hit(r,r_t_min,r_t_max,rec);
        bool hit_r = right->hit(r,r_t_min,hit_l ? rec.t : r_t_max,rec);

        return hit_l || hit_r;
    }

private:
    std::shared_ptr<hittable> left;
    std::shared_ptr<hittable> right;
    aabb bbox;

    static bool boxCompare(
        const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, int axis_index
    ) {
        auto a_axis_interval = a->boundingBox().axInt(axis_index);
        auto b_axis_interval = b->boundingBox().axInt(axis_index);
        return a_axis_interval.min < b_axis_interval.min;
    }

    static bool boxXCompare (const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
        return boxCompare(a, b, 0);
    }

    static bool boxYCompare (const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
        return boxCompare(a, b, 1);
    }

    static bool boxZCompare (const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
        return boxCompare(a, b, 2);
    }
};