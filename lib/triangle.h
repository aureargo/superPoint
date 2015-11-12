#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "global.h"
#include "ray.h"

class Triangle
{
public:
    const glm::vec3 p0, p1, p2;

    Triangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);

    float intersect(const Ray &ray) const;
    glm::vec3 getNormal() const;

};

#endif // TRIANGLE_H
