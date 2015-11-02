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

    void merge(const glm::vec3& p);
};

#endif // BOX_H
