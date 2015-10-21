#ifndef SPHERE_H
#define SPHERE_H

#include "primitive.h"

class Sphere : public Primitive
{
public:
    const float radius;
    const glm::vec3 center;

    Sphere(float radius, const glm::vec3& center);

    glm::vec3 getNormal(const glm::vec3& p) const;
    void reposition(glm::vec3& pos, const glm::vec3& n, const glm::vec3& dir, bool out) const;
    float intersect(const Ray &ray) const;
};

#endif // SPHERE_H
