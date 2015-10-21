#ifndef DIFFUSE_H
#define DIFFUSE_H

#include "material.h"

class Diffuse : public Material
{
public:
    Diffuse(const glm::vec3& color);
    glm::vec3 direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const;
    glm::vec3 indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const;
    glm::vec3 projection(const Ray& cam, const glm::vec3& n) const;
};

#endif // DIFFUSE_H
