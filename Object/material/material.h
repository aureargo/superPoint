#ifndef MATERIAL_H
#define MATERIAL_H

#include "global.h"
#include "../primitive/primitive.h"
#include "smallpt.h"


float speculaire(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, float brillance = BRILLANCE);
glm::vec3 monteCarlo(const glm::vec3& l, const glm::vec3& p);
bool aLaLumiere(const glm::vec3& p, const glm::vec3& l);
glm::vec3 radiance (const Ray & r, const glm::vec3& l, const int radMax);

float random_u();
glm::vec3 sample_cos(const float u, const float v, const glm::vec3& n);
bool refract(glm::vec3 i, glm::vec3 n, float ior, glm::vec3 &wo);
glm::vec3 reflect(const glm::vec3& i, const glm::vec3& n);  //montrer au prof parce qu'utilise la fonction de glm

class Material
{
public:
    const glm::vec3 color;

    Material(const glm::vec3& color = BLANC):
            color(color)
    {
    }



    virtual glm::vec3 direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const = 0;
    virtual glm::vec3 indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const = 0;
    virtual glm::vec3 projection(const Ray& cam, const glm::vec3& n) const = 0;

    Ray projection(const Ray& cam, const glm::vec3& p, const glm::vec3& n) const
    {
        return Ray(p, projection(cam,n));
    }
};

/*****************************************************************************************************************/


#include "diffuse.h"
#include "glass.h"
#include "mirror.h"

template<typename T>
glm::vec3 albedo(const T &t)
{
    return t.color;
}



#endif // MATERIAL_H
