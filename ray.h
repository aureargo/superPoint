#ifndef RAY_H
#define RAY_H

#include "global.h"

class Ray
{
public:
    Ray(const glm::vec3& origin, const glm::vec3& direction):
        origin(origin),     direction(direction)
    {
    }

    glm::vec3 origin, direction;
};

#endif // RAY_H
