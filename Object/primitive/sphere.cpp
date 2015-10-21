#include "sphere.h"

Sphere::Sphere(float radius, const glm::vec3& center):
        radius(radius), center(center)
{
}

glm::vec3 Sphere::getNormal(const glm::vec3& p) const    {
    return glm::normalize(p-center);
}
void Sphere::reposition(glm::vec3& pos, const glm::vec3& n, const glm::vec3& dir, bool out) const
{
    float r2 = radius;
    r2 += (out  ?   0.0001f   :   -0.0001f);

    pos = center + n*r2;
    (void) n;   (void) dir;
}

float Sphere::intersect(const Ray & ray) const
{				// returns distance, 0 if nohit
    glm::vec3 op = center - ray.origin;		// Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
    float t, b = glm:: dot(ray.direction, op), det =
        b * b - glm::dot(op, op) + radius*radius;
    if (det < 0)
        return noIntersect;
    else
        det = std::sqrt (det);
    return (t = b - det) >= 0 ? t : ((t = b + det) >= 0 ? t : noIntersect);
}
