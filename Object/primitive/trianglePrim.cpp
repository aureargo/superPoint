#include "trianglePrim.h"


TrianglePrim::TrianglePrim(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2):
        Triangle(p0,p1,p2)
{
}

glm::vec3 TrianglePrim::getNormal(const glm::vec3& p, const glm::vec3& dir, int id) const    {
    glm::vec3 n = Triangle::getNormal();
    if(distance2(n,dir) > 2)
        return -n;
    return n;
    (void) p;   (void) id;
}
void TrianglePrim::reposition(glm::vec3& pos, const glm::vec3& n, bool out) const
{
    if(!out)
        pos += n*0.0001f;
    else
        pos -= n*0.0001f;
}

float TrianglePrim::intersect(const Ray & ray, int& id) const
{
    return Triangle::intersect(ray);
    (void) id;
}
