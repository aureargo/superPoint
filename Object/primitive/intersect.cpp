#include "intersect.h"




float intersect(const Ray & ray, const Triangle &triangle)
{
    auto e1 = triangle.v1 - triangle.v0;
    auto e2 = triangle.v2 - triangle.v0;

    auto h = glm::cross(ray.direction, e2);
    auto a = glm::dot(e1, h);

    auto f = 1.f / a;
    auto s = ray.origin - triangle.v0;

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



float intersect(const Ray & ray, const Mesh &mesh)
{
    if(intersect(ray, mesh.box) == noIntersect)
        return noIntersect;

    float min = noIntersect;
    for(unsigned int i = 0;  i < mesh.faces.size();   i+=3)
    {
        Triangle t{mesh.vertices[mesh.faces[i]],mesh.vertices[mesh.faces[i+1]],mesh.vertices[mesh.faces[i+2]]};
        float val = intersect(ray, t);
        if(val < min)
            min = val;
    }
    return min;
}

