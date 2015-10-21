#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "global.h"
#include "ray.h"

class Primitive
{
public:
    virtual glm::vec3 getNormal(const glm::vec3& p) const = 0;
    virtual void reposition(glm::vec3& pos, const glm::vec3& n, const glm::vec3& dir, bool out) const = 0;
    virtual float intersect(const Ray& ray) const = 0;
};


/*********************************************************************************************/


#include "sphere.h"
#include "triangle.h"
#include "mesh.h"




#endif // PRIMITIVE_H
