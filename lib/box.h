#ifndef BOX_H
#define BOX_H

#include "global.h"
#include "ray.h"

class Box
{
public:
    glm::vec3 min, max;

    Box();
    Box(const glm::vec3& pos);
    Box(const glm::vec3& min, const glm::vec3& max);

    float intersect(const Ray& ray) const;
    bool intersect(const Ray & r, float& distanceMin, float& distanceMax) const;


    bool inOut(const glm::vec3& p) const;

    void merge(const glm::vec3& p);
    void merge(const Box& box);

    int axeMax() const;

private:
    inline float intersectIn(const Ray& r) const;
};

#endif // BOX_H
