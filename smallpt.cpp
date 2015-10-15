#ifndef SMALLPT_H
#define SMALLPT_H

// This code is highly based on smallpt
// http://www.kevinbeason.com/smallpt/
#include <cmath>
#include <algorithm>
#include <cassert>
#include <random>
#include <memory>
#include <fstream>
#include <iostream>

// GLM (vector / matrix)
#define GLM_FORCE_RADIANS

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtx/norm.hpp> //ajouté

const float pi = 3.1415927f;
const float noIntersect = std::numeric_limits<float>::infinity();

bool isIntersect(float t)
{
    return t < noIntersect;
}

#include "primitive.h"

    // WARRING: works only if r.d is normalized
float intersect (const Ray & ray, const Sphere &sphere)
{				// returns distance, 0 if nohit
    glm::vec3 op = sphere.center - ray.origin;		// Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
    float t, b = glm:: dot(ray.direction, op), det =
        b * b - glm::dot(op, op) + sphere.radius * sphere.radius;
    if (det < 0)
        return noIntersect;
    else
        det = std::sqrt (det);
    return (t = b - det) >= 0 ? t : ((t = b + det) >= 0 ? t : noIntersect);
}

float intersect(const Ray & ray, const Triangle &triangle)
{
    auto e1 = triangle.v1 - triangle.v0;
    auto e2 = triangle.v2 - triangle.v0;

    auto h = glm::cross(ray.direction, e2);
    auto a = glm::dot(e1, h);

    auto f = 1.f / a;
    auto s = ray.origin - triangle.v0;

    auto u = f * glm::dot(s, h);
    auto q = glm::cross(s, e1);
    auto v = f * glm::dot(ray.direction, q);
    auto t = f * glm::dot(e2, q);

    if(std::abs(a) < 0.00001)
        return noIntersect;
    if(u < 0 || u > 1)
        return noIntersect;
    if(v < 0 || (u+v) > 1)
        return noIntersect;
    if(t < 0)
        return noIntersect;

    return t;
}

float intersect(const Ray & r, const Box &box)
{
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    const glm::vec3& min = box.min;
    const glm::vec3& max = box.max;
    float distanceMin = noIntersect;

    float div;

    if(r.direction.x == 0)    {
        tmin = FLT_MIN;
        tmax = FLT_MAX;
    }
    else if(r.direction.x > 0)    {
        div = 1 / r.direction.x;
        tmin = (min.x - r.origin.x) * div;
        tmax = (max.x - r.origin.x) * div;
    }
    else    {
        div = 1 / r.direction.x;
        tmin = (max.x - r.origin.x) * div;
        tmax = (min.x - r.origin.x) * div;
    }

    if(r.direction.y == 0)    {
        tymin = FLT_MIN;
        tymax = FLT_MAX;
    }
    else if(r.direction.y >= 0)    {
        div = 1 / r.direction.y;
        tymin = (min.y - r.origin.y) * div;
        tymax = (max.y - r.origin.y) * div;
    }
    else    {
        div = 1 / r.direction.y;
        tymin = (max.y - r.origin.y) * div;
        tymax = (min.y - r.origin.y) * div;
    }

    if( (tmin > tymax) || (tymin > tmax) )
        return noIntersect;

    if(tymin > tmin)
        tmin = tymin;

    if(tymax < tmax)
        tmax = tymax;


    if(r.direction.z == 0)    {
        tzmin = FLT_MIN;
        tzmax = FLT_MAX;
    }
    else if(r.direction.z > 0)    {
        div = 1 / r.direction.z;
        tzmin = (min.z - r.origin.z) * div;
        tzmax = (max.z - r.origin.z) * div;
    }
    else    {
        div = 1 / r.direction.z;
        tzmin = (max.z - r.origin.z) * div;
        tzmax = (min.z - r.origin.z) * div;
    }

    if( (tmin > tzmax) || (tzmin > tmax) )
        return noIntersect;

    if(tzmin > tmin)
        tmin = tzmin;

    if(tzmax < tmax)
        tmax = tzmax;

    if(tmin>=0)
        distanceMin = tmin;
    return distanceMin;
    //else
    //    return false; //inutile apparament
    //distanceMin += 0.002;

    //if(tmax>0)
    //    distanceMax = tmax;

    return true;
}

float intersect(const Ray & ray, const Mesh &mesh)
{
    if(intersect(ray, mesh.box) == noIntersect)
        return noIntersect;

    float min = noIntersect;
    for(unsigned int i = 0;  i < mesh.faces.size();   i+=3)
    {
        Triangle t{mesh.vertices[mesh.faces[i]],mesh.vertices[mesh.faces[i+1]],mesh.vertices[mesh.faces[i+2]]};
        float val = intersect(ray, t);
        if(val < min)
            min = val;
    }
    return min;
}

/*************************************************************/

#include "Object.h"

/*************************************************************/

// Scene
#include "scene.h"

/*************************************************************/

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

#define NEAR_INTERSECT 0.1f

// WARNING: ASSUME NORMALIZED RAY
// Compute the intersection ray / scene.
// Returns true if intersection
// t is defined as the abscisce along the ray (i.e
//             p = r.o + t * r.d
// id is the id of the intersected object
Object* intersect (const Ray & r, float &t)
{
    t = noIntersect;
    Object *ret = nullptr;

    for(auto &object : scene::objects)
    {
        float d = object->intersect(r);
        //if (isIntersect(d) && d < t)
        if (d < t && d > NEAR_INTERSECT)  //pas d>=0.0f pour permettre la diffusion à partir d'un objet de la scène sans que la fonction retourne le même objet
        {
            t = d;
            ret = object.get();
        }
    }

    return ret;
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



/*************************************************************/
#define SQR_PRECISION 0.04f
#define AMBIANTE 0.0f
//0.03125f
#define BRILLANCE 16


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

glm::vec3 radiance (const Ray & r, const int radMax = 5)
{
    float d;
    const Object* obj = intersect(r, d);
    if(obj == nullptr)
        return glm::vec3(0,0,0);



    glm::vec3 pos = r.origin + r.direction*d;
    glm::vec3 n = obj->getNormal(pos);

    glm::vec3 color = obj->direct(r, pos, n, scene::light);
    if(radMax > 0)
        color += obj->indirect(r, pos, n, scene::light, radMax-1);

    return color;




}

/*************************************************************/
/*************************************************************/
/*************************************************************/

#include "convolution.h"
#include "lumiere.h"

/*************************************************************/

int main (int, char **)
{
	int w = 768, h = 768;
    std::vector<glm::vec3> colors(w * h, glm::vec3{0.f, 0.f, 0.f});

	Ray cam {{50, 52, 295.6}, glm::normalize(glm::vec3{0, -0.042612, -1})};	// cam pos, dir
	float near = 1.f;
	float far = 10000.f;


    std::vector<lux> lums;
    lums.reserve(10000);
    for(int i = 0;  i < 10000;  i++)
    {
        float puissance = 2.f;
        glm::vec3 dir(random_u(), random_u(), random_u());
        dir = glm::normalize(dir);
        float t;
        Ray ray = Ray{scene::light, dir};
        Object* obj = intersect(ray ,t);
        if(obj != nullptr)
        {
            float newPuissance = puissance*0.5;
            if(newPuissance > 0.01f)
            {
                glm::vec3 pos = scene::light+dir*t;
                lums.push_back(lux{newPuissance,pos});

                ray = Ray{pos, obj->projection(ray, obj->getNormal(pos))};
                while(newPuissance > 0.01f)
                {
                    obj = intersect(ray,t);
                    if(obj != nullptr)
                    {
                        newPuissance = newPuissance*0.5;
                        if(newPuissance > 0.01f)
                        {
                            pos = pos+dir*t;
                            lums.push_back(lux{newPuissance,pos});
                            ray = Ray{pos, obj->projection(ray, obj->getNormal(pos))};
                        }
                    }
                    else
                        break;
                }
            }
        }
    }

    lums.shrink_to_fit();
    std::sort(lums.begin(),	lums.end());





	glm::mat4 camera =
		glm::scale(glm::mat4(1.f), glm::vec3(float(w), float(h), 1.f))
		* glm::translate(glm::mat4(1.f), glm::vec3(0.5, 0.5, 0.f))
		* glm::perspective(float(54.5f * pi / 180.f), float(w) / float(h), near, far)
		* glm::lookAt(cam.origin, cam.origin + cam.direction, glm::vec3(0, 1, 0))
		;

	glm::mat4 screenToRay = glm::inverse(camera);



    const unsigned int nbRayons = 4;
    int prct, prct2 = -1;
    //#pragma omp parallel for
	for (int y = 0; y < h; y++)
    {
        prct = 100 * y / (h - 1);
        if(prct != prct2)
        {
            std::cerr << "\rRendering: " << prct << "%";
            prct2 = prct;
        }

        #pragma omp parallel for
		for (unsigned short x = 0; x < w; x++)
		{
			glm::vec4 p0 = screenToRay * glm::vec4{float(x), float(h - y), 0.f, 1.f};
			glm::vec4 p1 = screenToRay * glm::vec4{float(x), float(h - y), 1.f, 1.f};

			glm::vec3 pp0 = glm::vec3(p0 / p0.w);
			glm::vec3 pp1 = glm::vec3(p1 / p1.w);

			glm::vec3 d = glm::normalize(pp1 - pp0);
            glm::vec3 r(0,0,0);
            for(unsigned int i = 0; i < nbRayons;  i++)
            {
                glm::vec3 d2(sample_cos(random_u(),random_u(), d));
                d2 /= glm::vec3(w,h,w*h);
                d2 += d;
                d2 = glm::normalize(d2);
                r += radiance (Ray{pp0, d2}, 0);
            //r += radiance (Ray{pp0, d}, 20);
            }
            r /= nbRayons;
            colors[y * w + x] += glm::clamp(r, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
		}
    }

    //convolution
    for(int i = 0;  i < 0;  i++)
        colors = convolution(colors, h, w);

	{
		std::fstream f("image.ppm", std::fstream::out);
		f << "P3\n" << w << " " << h << std::endl << "255" << std::endl;

        for (auto c : colors)
			f << toInt(c.x) << " " << toInt(c.y) << " " << toInt(c.z) << " ";
    }
}

#endif
