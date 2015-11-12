#include "Object/material/diffuse.h"

Diffuse::Diffuse(const glm::vec3& color):
        Material(color)
{
}

glm::vec3 Diffuse::direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const Lumiere& l) const
{
    const glm::vec3 l2 = monteCarlo(l,p);
    if(!aLaLumiere(p, l2))
        return NOIR;
    float diffuse = fabsf(glm::dot(n, glm::normalize(l2-p))) / pi;
    float specular = speculaire(cam, p, n, l, brillance);
    return (color * (l.absDiffus*absDiffus*diffuse+l.absSpeculaire*absSpeculaire*specular));
}

glm::vec3 Diffuse::indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const Lumiere& l, int radMax) const
{
    /*glm::vec3 newDir = reflect(-cam.direction,n);
    glm::vec3 c = radiance(Ray(p, newDir), l, radMax);
    return c*l.absDiffus*absDiffus*color;*/

    glm::vec3 w = projection(cam, n);
    glm::vec3 c = radiance(Ray(p,w), l, radMax-1)*fabsf(glm::dot(n,w));      // fabsf(glm::dot(n,w)) == fabsf(glm::dot(-n,w))

    return color*l.absDiffus*absDiffus*c;
    //return NOIR;
}


glm::vec3 Diffuse::projection(const Ray& cam, const glm::vec3& n) const
{
    float angle = glm::dot(n, cam.direction);   // normalement glm::dot(n, -cam.direction) mais on inverse tout après
    return glm::vec3(sample_cos(random_u(),random_u(),  (angle < 0    ?   n   :   -n) ));
}
