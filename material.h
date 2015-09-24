#ifndef MATERIAL_H
#define MATERIAL_H

#define NOIR glm::vec3(0.f, 0.f, 0.f)

bool aLaLumiere(const glm::vec3& p, const glm::vec3& l);
glm::vec3 radiance (const Ray & r, int radMax);
bool refract(glm::vec3 i, glm::vec3 n, float ior, glm::vec3 &wo);


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
        (void) c;   (void) p;   (void) n;   (void) l;   (void) radMax;
    }
};

/*************************************************************************************/

struct Glass    {
    const glm::vec3 color;
    glm::vec3 direct(const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const
    {
        return NOIR;
        (void) p;   (void) n;   (void) l;
    }

    glm::vec3 indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const
    {
        glm::vec3 wo;
        if(!refract(cam.direction, n, 1.33, wo))
            wo = reflect(cam.direction,n);
        else
            wo = -wo;

        glm::vec3 c = radiance(Ray{p, wo}, radMax);
        return glm::vec3(c.r*color.r, c.g*color.g, c.b*color.b);
        //return c;
        (void) l;
    }
};

/*************************************************************************************/

struct Mirror   {
    const glm::vec3 color;
    glm::vec3 direct(const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const
    {
        return NOIR;
        (void) p;   (void) n;   (void) l;
    }

    glm::vec3 indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const
    {
        glm::vec3 newDir = reflect(cam.direction,n);
        glm::vec3 c = radiance(Ray{p, newDir}, radMax);
        return glm::vec3(c.r*color.r, c.g*color.g, c.b*color.b);
        (void) l;
    }
};

/*************************************************************/

template<typename T>
glm::vec3 albedo(const T &t)
{
    return t.color;
}



#endif // MATERIAL_H
