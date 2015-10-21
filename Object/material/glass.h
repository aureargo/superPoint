#ifndef GLASS_H
#define GLASS_H

#include "material.h"

class Glass : public Material
{
public:
    const float refractProp;

    Glass(const glm::vec3& color, float refractProp = 1.f);

    glm::vec3 direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const;
    glm::vec3 indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const;
    glm::vec3 projection(const Ray& cam, const glm::vec3& n) const;

};

#endif // GLASS_H
