#ifndef SPHERE_H
#define SPHERE_H

#include "primitive.h"

class Sphere : public Primitive
{
public:
    const float radius;
    const glm::vec3 center;

    Sphere(float radius, const glm::vec3& center);

    glm::vec3 getNormal(const glm::vec3& p, const glm::vec3& dir, int id) const;
    void reposition(glm::vec3& pos, const glm::vec3& n, bool out) const;
    float intersect(const Ray &ray, int& id) const;
};

#endif // SPHERE_H
