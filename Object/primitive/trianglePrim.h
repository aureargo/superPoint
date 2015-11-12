#ifndef TRIANGLE_PRIM_H
#define TRIANGLE_PRIM_H

#include "primitive.h"
#include "lib/triangle.h"

class TrianglePrim: public Triangle, public Primitive
{
public:

    TrianglePrim(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);

    glm::vec3 getNormal(const glm::vec3& p, const glm::vec3& dir, int id) const;
    void reposition(glm::vec3& pos, const glm::vec3& n, bool out) const;
    float intersect(const Ray &ray, int& id) const;
};

#endif // TRIANGLE_PRIM_H
