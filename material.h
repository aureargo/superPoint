#ifndef MATERIAL_H
#define MATERIAL_H

#define NOIR glm::vec3(0.f, 0.f, 0.f)

bool aLaLumiere(const glm::vec3& p, const glm::vec3& l);
glm::vec3 radiance (const Ray & r, int radMax);


struct Diffuse  {
    const glm::vec3 color;
    glm::vec3 direct(const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const
    {
        if(!aLaLumiere(p,l))
            return NOIR;
        return (color * fabsf(glm::dot(n, glm::normalize(l-p)))) / pi;
    }

    glm::vec3 indirect(const Ray& c, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const
    {
        return NOIR;
    }
};

/*************************************************************************************/

struct Glass    {
    const glm::vec3 color;
    glm::vec3 direct(const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const
    {
        return NOIR;
    }

    glm::vec3 indirect(const Ray& c, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const
    {
        return NOIR;
    }
};

/*************************************************************************************/

struct Mirror   {
    const glm::vec3 color;
    glm::vec3 direct(const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const
    {
        return NOIR;
    }

    glm::vec3 indirect(const Ray& c, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const
    {
        glm::vec3 newDir = reflect(c.direction,n);
        return radiance(Ray{p, newDir}, radMax);
        //newColor *= diffuse;
        //c += newColor;
        //return NOIR;
    }
};

/*************************************************************/

template<typename T>
glm::vec3 albedo(const T &t)
{
    return t.color;
}



#endif // MATERIAL_H
