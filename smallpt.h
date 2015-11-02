#ifndef SMALLPT_H
#define SMALLPT_H

#include "ray.h"
//#include "lumiere.h"
#include "convolution.h"
#include "global.h"
#include "Object/Object.h"


float random_u();
glm::vec3 sample_cos(const float u, const float v, const glm::vec3& n);
int toInt (float x);


// Reflect the ray i along the normal.
// i should be oriented as "leaving the surface"
glm::vec3 reflect(const glm::vec3& i, const glm::vec3& n);
float sin2cos (float x);

// Fresnel coeficient of transmission.
// Normal point outside the surface
// ior is n0 / n1 where n0 is inside and n1 is outside
float fresnelR(const glm::vec3& i, const glm::vec3& n, float ior);

// compute refraction vector.
// return true if refraction is possible.
// i and n are normalized
// output wo, the refracted vector (normalized)
// n point oitside the surface.
// ior is n00 / n1 where n0 is inside and n1 is outside
//
// i point outside of the surface
bool refract(glm::vec3 i, glm::vec3 n, float ior, glm::vec3 &wo);

glm::vec3 sample_sphere(float r, float u, float v, float &pdf, const glm::vec3& normal);


/*************************************************************/



float speculaire(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const Lumiere& l, float brillance = BRILLANCE);
glm::vec3 monteCarlo(const Lumiere& l, const glm::vec3& p);

bool aLaLumiere(const glm::vec3& p, const Lumiere& l);

glm::vec3 radiance (const Ray & r, const Lumiere& l, const int radMax = 5);


#endif // SMALLPT_H
