#include "glass.h"

Glass::Glass(const glm::vec3& color, float refractProp):
        Material(color),    refractProp(refractProp)
{
}

glm::vec3 Glass::direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const
{
    /*if(!aLaLumiere(p,l))
        return NOIR;
    float diffuse = fabsf(glm::dot(n, glm::normalize(l-p))) / pi;
    return (color * diffuse);*/

    if(aLaLumiere(p,monteCarlo(l,p)))
        return color*speculaire(cam, p, n, l);
    else
        //return color;
        return NOIR;
}

glm::vec3 Glass::indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const
{
    glm::vec3 c = radiance(Material::projection(cam, p, n), l, radMax);
    return c*color;//glm::vec3(c.r*color.r, c.g*color.g, c.b*color.b);
    //return c;
}


glm::vec3 Glass::projection(const Ray& cam, const glm::vec3& n) const
{
    glm::vec3 wo;
    if(!refract(-cam.direction, n, refractProp, wo))
        wo = reflect(-cam.direction,n);
    return wo;
}
