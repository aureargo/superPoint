#include "box.h"

Box::Box():
        min(glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX)),    max(glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN))
{
}

Box::Box(const glm::vec3& pos):
        min(pos),   max(pos)
{
}


Box::Box(const glm::vec3& min, const glm::vec3& max):
        min(min),   max(max)
{
}

float Box::intersect(const Ray & r) const
{
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    float distanceMin = noIntersect;

    float div;

    if(r.direction.x == 0)    {
        tmin = FLT_MIN;
        tmax = FLT_MAX;
    }
    else if(r.direction.x > 0)    {
        div = 1 / r.direction.x;
        tmin = (min.x - r.origin.x) * div;
        tmax = (max.x - r.origin.x) * div;
    }
    else    {
        div = 1 / r.direction.x;
        tmin = (max.x - r.origin.x) * div;
        tmax = (min.x - r.origin.x) * div;
    }

    if(r.direction.y == 0)    {
        tymin = FLT_MIN;
        tymax = FLT_MAX;
    }
    else if(r.direction.y >= 0)    {
        div = 1 / r.direction.y;
        tymin = (min.y - r.origin.y) * div;
        tymax = (max.y - r.origin.y) * div;
    }
    else    {
        div = 1 / r.direction.y;
        tymin = (max.y - r.origin.y) * div;
        tymax = (min.y - r.origin.y) * div;
    }

    if( (tmin > tymax) || (tymin > tmax) )
        return noIntersect;

    if(tymin > tmin)
        tmin = tymin;

    if(tymax < tmax)
        tmax = tymax;


    if(r.direction.z == 0)    {
        tzmin = FLT_MIN;
        tzmax = FLT_MAX;
    }
    else if(r.direction.z > 0)    {
        div = 1 / r.direction.z;
        tzmin = (min.z - r.origin.z) * div;
        tzmax = (max.z - r.origin.z) * div;
    }
    else    {
        div = 1 / r.direction.z;
        tzmin = (max.z - r.origin.z) * div;
        tzmax = (min.z - r.origin.z) * div;
    }

    if( (tmin > tzmax) || (tzmin > tmax) )
        return noIntersect;

    if(tzmin > tmin)
        tmin = tzmin;

    if(tzmax < tmax)
        tmax = tzmax;

    if(tmin>=0)
        distanceMin = tmin;
    return distanceMin;
    //else
    //    return false; //inutile apparament
    //distanceMin += 0.002;

    //if(tmax>0)
    //    distanceMax = tmax;

    return true;
}
