#ifndef MIRROR_H
#define MIRROR_H

#include "material.h"

class Mirror : public Material
{
public:
    Mirror(const glm::vec3& color);

    glm::vec3 direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const;
    glm::vec3 indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const;
    glm::vec3 projection(const Ray& cam, const glm::vec3& n) const;
};


#endif // MIRROR_H
