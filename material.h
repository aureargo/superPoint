#ifndef MATERIAL_H
#define MATERIAL_H

#include "smallpt.cpp"

#define NOIR glm::vec3(0.f, 0.f, 0.f)

bool aLaLumiere(const glm::vec3& p, const glm::vec3& l);
glm::vec3 radiance (const Ray & r, int radMax);
bool refract(glm::vec3 i, glm::vec3 n, float ior, glm::vec3 &wo);
glm::vec3 reflect(const glm::vec3& i, const glm::vec3& n);
glm::vec3 sample_cos(const float u, const float v, const glm::vec3& n);
float random_u();


struct Diffuse  {
    const glm::vec3 color;
    glm::vec3 direct(const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const
    {
        if(!aLaLumiere(p,l))
            return NOIR;
        float diffuse = fabsf(glm::dot(n, glm::normalize(l-p))) / pi;
        return (color * diffuse);
    }

    glm::vec3 indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const
    {
        //glm::vec3 w = reflect(-cam.direction, n);
        glm::vec3 c(0.f,0.f,0.f);
        for(int i = 0;  i < 10; i++)
        {
            glm::vec3 w(sample_cos(random_u(),random_u(), n));
            c += radiance(Ray{p,w}, radMax-1);
        }

        return color*(c/20.f);

        return NOIR;
        (void) cam;   (void) p;   (void) n;   (void) l;   (void) radMax;
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
        if(!refract(cam.direction, n, 1/1.33, wo))
            wo = reflect(cam.direction,n);
        else
            wo = -wo;


        glm::vec3 c = radiance(Ray{p, wo}, radMax);
        return c*color;//glm::vec3(c.r*color.r, c.g*color.g, c.b*color.b);
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
        glm::vec3 newDir = reflect(-cam.direction,n);
        glm::vec3 c = radiance(Ray{p, newDir}, radMax);
        return c*color;//glm::vec3(c.r*color.r, c.g*color.g, c.b*color.b);
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
