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
        (void) p;
    }
    void reposition(glm::vec3& pos) const
    {
        (void) pos;
    }
};


/****************************************************************************************/


// WARRING: works only if r.d is normalized
float intersect (const Ray & ray, const Sphere &sphere)
{				// returns distance, 0 if nohit
    glm::vec3 op = sphere.center - ray.origin;		// Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
    float t, b = glm:: dot(ray.direction, op), det =
        b * b - glm::dot(op, op) + sphere.radius * sphere.radius;
    if (det < 0)
        return noIntersect;
    else
        det = std::sqrt (det);
    return (t = b - det) >= 0 ? t : ((t = b + det) >= 0 ? t : noIntersect);
}

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


#endif // PRIMITIVE_H
