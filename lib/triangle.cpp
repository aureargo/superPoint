#include "lib/triangle.h"

Triangle::Triangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2):
        p0(p0), p1(p1), p2(p2)
{
}

glm::vec3 Triangle::getNormal() const    {
    return glm::normalize(glm::cross(p1-p0, p2-p0));
}

float Triangle::intersect(const Ray& ray) const
{
    auto e1 = p1 - p0;
    auto e2 = p2 - p0;

    auto h = glm::cross(ray.direction, e2);
    auto a = glm::dot(e1, h);

    auto f = 1.f / a;
    auto s = ray.origin - p0;

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
