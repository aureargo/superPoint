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
    float distanceMin, distanceMax;
    if(intersect(r, distanceMin, distanceMax))
        return distanceMin;
    return noIntersect;
}
bool Box::intersect(const Ray & r, float& distanceMin, float& distanceMax) const
{
    if(this->inOut(r.origin))  {
        distanceMin = 0;
        distanceMax = intersectIn(r);
        return true;
    }

    float tmin, tmax, tymin, tymax, tzmin, tzmax;

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
        return false;

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
        return false;

    if(tzmin > tmin)
        tmin = tzmin;

    if(tzmax < tmax)
        tmax = tzmax;

    if(tmin>=0)
        distanceMin = tmin;
    else
        return false; //inutile apparament
    //distanceMin += 0.002;

    if(tmax>0)
        distanceMax = tmax;

    return true;
}

inline float Box::intersectIn(const Ray& r) const
{
    float tmax, tymax, tzmax;

    if(r.direction.x == 0)
        tmax = FLT_MAX;
    else if(r.direction.x > 0)
        tmax = (max.x - r.origin.x) / r.direction.x;
    else
        tmax = (min.x - r.origin.x) / r.direction.x;

    if(r.direction.y == 0)
        tymax = FLT_MAX;
    else if(r.direction.y >= 0)
        tymax = (max.y - r.origin.y) / r.direction.y;
    else
        tymax = (min.y - r.origin.y) / r.direction.y;

    if(tymax < tmax)
        tmax = tymax;


    if(r.direction.z == 0)
        return tmax;
    else if(r.direction.z > 0)
        tzmax = (max.z - r.origin.z) / r.direction.z;
    else
        tzmax = (min.z - r.origin.z) / r.direction.z;

    if(tzmax < tmax)
        return tzmax;
    return tmax;
}


bool Box::inOut(const glm::vec3& p) const
{
    for(int i = 0;  i < 3;  i++)
    {
        if(p[i] < min[i])
            return false;
        if(p[i] > max[i])
            return false;
    }
    return true;
}

void Box::merge(const glm::vec3& p)
{
    for(int i = 0;  i < 3;  i++)
    {
        if(min[i] > p[i])
            min[i] = p[i];
        if(max[i] < p[i])
            max[i] = p[i];
    }
}

void Box::merge(const Box& box)
{
    for(int i = 0;  i < 3;  i++)
    {
        if(min[i] > box.min[i])
            min[i] = box.min[i];
        if(max[i] < box.max[i])
            max[i] = box.max[i];
    }
}


int Box::axeMax() const
{
    float   x = max.x - min.x,
            y = max.y - min.y,
            z = max.z - min.z;
    if(x >= y)   {
        if(x >= z)  return 0;
        else        return 2;
    }
    else if(y >= z)
        return 1;
    else
        return 2;
}
