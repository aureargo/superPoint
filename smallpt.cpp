#include "smallpt.h"


bool isIntersect(float t)
{
    return t < noIntersect;
}


//thread_local std::default_random_engine generator;
//thread_local std::uniform_real_distribution<float> distribution(0.0,1.0);
std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0.0,1.0);

float random_u()
{
	return distribution(generator);
}

glm::vec3 sample_cos(const float u, const float v, const glm::vec3& n)
{
	// Ugly: create an ornthogonal base
	glm::vec3 basex, basey, basez;

	basez = n;
	basey = glm::vec3(n.y, n.z, n.x);

	basex = glm::cross(basez, basey);
	basex = glm::normalize(basex);

	basey = glm::cross(basez, basex);

	// cosinus sampling. Pdf = cosinus
	return  basex * (std::cos(2.f * pi * u) * std::sqrt(1.f - v)) +
		basey * (std::sin(2.f * pi * u) * std::sqrt(1.f - v)) +
		basez * std::sqrt(v);
}

int toInt (float x){
	return int (std::pow (glm::clamp (x, 0.f, 1.f), 1.f / 2.2f) * 255 + .5);
}



// Reflect the ray i along the normal.
// i should be oriented as "leaving the surface"
glm::vec3 reflect(const glm::vec3& i, const glm::vec3& n){
    return n * (glm::dot(n, i)) * 2.f - i;
}

float sin2cos (float x){
	return std::sqrt(std::max(0.0f, 1.0f-x*x));
}

// Fresnel coeficient of transmission.
// Normal point outside the surface
// ior is n0 / n1 where n0 is inside and n1 is outside
float fresnelR(const glm::vec3& i, const glm::vec3& n, float ior)
{
	if(glm::dot(n, i) < 0)
		return fresnelR(i, n * -1.f, 1.f / ior);

	float R0 = (ior - 1.f) / (ior + 1.f);
	R0 *= R0;

	return R0 + (1.f - R0) * std::pow(1.f - glm::dot(i, n), 5.f);
}

// compute refraction vector.
// return true if refraction is possible.
// i and n are normalized
// output wo, the refracted vector (normalized)
// n point oitside the surface.
// ior is n00 / n1 where n0 is inside and n1 is outside
//
// i point outside of the surface
bool refract(glm::vec3 i, glm::vec3 n, float ior, glm::vec3 &wo)
{
    i = -i;

	if(glm::dot(n, i) > 0)
        n = -n;
	else
		ior = 1.f / ior;

    float k = 1.f - ior * ior * (1.f - glm::dot(n, i) * glm::dot(n, i));
    if (k < 0.f)
		return false;

	wo = i * ior - n * (ior * glm::dot(n, i) + std::sqrt(k));

	return true;
}

glm::vec3 sample_sphere(float r, float u, float v, float &pdf, const glm::vec3& normal)
{
	pdf = 1.f / (pi * r * r);
	glm::vec3 sample_p = sample_cos(u, v, normal);

	float cos = glm::dot(sample_p, normal);

	pdf *= cos;
	return sample_p * r;
}


float speculaire(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, float brillance)
{
    const glm::vec3 R = reflect(glm::normalize(l-p),n);
    return std::pow(fabsf(glm::dot(R,-cam.direction)), brillance);
}

glm::vec3 monteCarlo(const glm::vec3& l, const glm::vec3& p)
{
    return l+4.f*sample_cos(random_u(),random_u(),glm::normalize(l-p));
}

struct Object;
Object* intersect (const Ray & r, float &t);

bool aLaLumiere(const glm::vec3& p, const glm::vec3& l)
{
    float sqrdist = glm::distance2(p,l);

    Ray rl = {l, normalize(p-l)};

    float d2;
    const Object* obj2 = intersect(rl, d2);

    if(obj2 == nullptr || fabsf(d2*d2-sqrdist) > SQR_PRECISION)
        return false;
    return true;
}

glm::vec3 radiance (const Ray & r, const glm::vec3 &l, const int radMax)
{
    float d;
    const Object* obj = intersect(r, d);
    if(obj == nullptr)
        return NOIR;



    glm::vec3 pos = r.origin + r.direction*d;
    glm::vec3 n = obj->getNormal(pos);

    glm::vec3 color = obj->direct(r, pos, n, l);
    if(radMax > 0)
        color += obj->indirect(r, pos, n, l, radMax-1);

    return color;
}


