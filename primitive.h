#ifndef PRIMITIVE_H
#define PRIMITIVE_H

struct Ray
{
    const glm::vec3 origin, direction;
};

struct Sphere
{
    const float radius;
    const glm::vec3 center;

    glm::vec3 getNormal(const glm::vec3& p) const    {
        return glm::normalize(p-center);
    }
    void reposition(glm::vec3& pos) const
    {
        float sqrd = distance2(center, pos);
        float sqrR = radius*radius;
        if(sqrd < sqrR)
            pos *= sqrR/sqrd;
    }
};

struct Triangle
{
    const glm::vec3 v0, v1, v2;

    glm::vec3 getNormal(const glm::vec3& p) const    {
        return glm::normalize(glm::cross(v1-v0, v2-v0));
    }
    void reposition(glm::vec3& pos) const
    {

    }
};

#endif // PRIMITIVE_H
