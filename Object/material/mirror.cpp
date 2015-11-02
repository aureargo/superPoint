#include "mirror.h"

Mirror::Mirror(const glm::vec3& color):
        Material(color)
{
}

glm::vec3 Mirror::direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const Lumiere& l) const
{
    /*if(!aLaLumiere(p,l))
        return NOIR;
    float diffuse = fabsf(glm::dot(n, glm::normalize(l-p))) / pi;
    return (color * diffuse);*/

    if(aLaLumiere(p,monteCarlo(l,p)))
        return color*l.absSpeculaire*absSpeculaire*speculaire(cam, p, n, l, brillance);
    else
        //return color;
        return NOIR;
}

glm::vec3 Mirror::indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const Lumiere& l, int radMax) const
{
    glm::vec3 newDir = reflect(-cam.direction,n);
    glm::vec3 c = radiance(Ray(p, newDir), l, radMax);
    return c*l.absDiffus*absDiffus*color;
}

glm::vec3 Mirror::projection(const Ray& cam, const glm::vec3& n) const
{
    return reflect(-cam.direction,n);
}
