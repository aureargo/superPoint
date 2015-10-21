#include "triangle.h"


Triangle::Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2):
        v0(v0), v1(v1), v2(v2)
{
}

glm::vec3 Triangle::getNormal(const glm::vec3& p) const    {
    return glm::normalize(glm::cross(v1-v0, v2-v0));
    (void) p;
}
void Triangle::reposition(glm::vec3& pos, const glm::vec3& n, const glm::vec3& dir, bool out) const
{
    if(distance2(n,dir) > 2)
        out = !out;

    (void) pos;
}

float Triangle::intersect(const Ray & ray) const
{
    auto e1 = v1 - v0;
    auto e2 = v2 - v0;

    auto h = glm::cross(ray.direction, e2);
    auto a = glm::dot(e1, h);

    auto f = 1.f / a;
    auto s = ray.origin - v0;

    auto u = f * glm::dot(s, h);
    auto q = glm::cross(s, e1);
    auto v = f * glm::dot(ray.direction, q);
    auto t = f * glm::dot(e2, q);

    if(std::abs(a) < 0.00001)
        return noIntersect;
    if(u < 0 || u > 1)
        return noIntersect;
    if(v < 0 || (u+v) > 1)
        return noIntersect;
    if(t < 0)
        return noIntersect;

    return t;
}
