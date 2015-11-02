#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "primitive.h"

class Triangle: public Primitive
{
public:
    const glm::vec3 v0, v1, v2;

    Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

    glm::vec3 getNormal(const glm::vec3& p, const glm::vec3& dir, int id) const;
    void reposition(glm::vec3& pos, const glm::vec3& n, bool out) const;
    float intersect(const Ray &ray, int& id) const;
};

#endif // TRIANGLE_H
