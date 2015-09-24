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
    void reposition(glm::vec3& pos, const glm::vec3& n, const glm::vec3& dir, bool out) const
    {
        float r2 = radius;
        r2 += (out  ?   0.0001f   :   -0.0001f);

        pos = center + n*r2;
        (void) n;   (void) dir;
    }
};

struct Triangle
{
    const glm::vec3 v0, v1, v2;

    glm::vec3 getNormal(const glm::vec3& p) const    {
        return glm::normalize(glm::cross(v1-v0, v2-v0));
        (void) p;
    }
    void reposition(glm::vec3& pos, const glm::vec3& n, const glm::vec3& dir, bool out) const
    {
        if(distance2(n,dir) > 2)
            out = !out;

        (void) pos;
    }
};

#endif // PRIMITIVE_H
