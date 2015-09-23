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

struct Ray
{
	const glm::vec3 origin, direction;
};

struct Sphere
{
	const float radius;
	const glm::vec3 center;

    glm::vec3 getNormal(const glm::vec3& p) const    {
        return glm::normalize(p-center);
    }
};

struct Triangle
{
	const glm::vec3 v0, v1, v2;

    glm::vec3 getNormal(const glm::vec3& p) const    {
        return glm::normalize(glm::cross(v1-v0, v2-v0));
    }
};

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

/*************************************************************/

struct Diffuse  {
	const glm::vec3 color;
};

struct Glass    {
	const glm::vec3 color;
};

struct Mirror   {
	const glm::vec3 color;
};

/*************************************************************/

template<typename T>
glm::vec3 albedo(const T &t)
{
	return t.color;
}

/*************************************************************/

struct Object
{
	virtual float intersect(const Ray &r) const = 0;
	virtual glm::vec3 albedo() const = 0;
    virtual glm::vec3 getNormal(const glm::vec3& pos) const = 0;
    //cos(theta)/pi

};

template<typename P, typename M>
struct ObjectTpl final : Object
{
	ObjectTpl(const P &_p, const M &_m)
		:primitive(_p), material(_m)
	{}

	float intersect(const Ray &ray) const	{
		return ::intersect(ray, primitive);
	}

	glm::vec3 albedo() const	{
		return ::albedo(material);
	}

    glm::vec3 getNormal(const glm::vec3& p) const    {
        return primitive.getNormal(p);
    }

	const P &primitive;
	const M &material;
};


template<typename P, typename M>
std::unique_ptr<Object> makeObject(const P&p, const M&m){
	return std::unique_ptr<Object>(new ObjectTpl<P, M>{p, m});
}

/*************************************************************/

// Scene
namespace scene
{
	// Primitives

	// Left Wall
    const Triangle leftWallA{{0, 0, 0}, {0, 100, 0}, {0, 0, 150}};
	const Triangle leftWallB{{0, 100, 150}, {0, 100, 0}, {0, 0, 150}};

	// Right Wall
	const Triangle rightWallA{{100, 0, 0}, {100, 100, 0}, {100, 0, 150}};
	const Triangle rightWallB{{100, 100, 150}, {100, 100, 0}, {100, 0, 150}};

	// Back wall
	const Triangle backWallA{{0, 0, 0}, {100, 0, 0}, {100, 100, 0}};
	const Triangle backWallB{{0, 0, 0}, {0, 100, 0}, {100, 100, 0}};

	// Bottom Floor
	const Triangle bottomWallA{{0, 0, 0}, {100, 0, 0}, {100, 0, 150}};
	const Triangle bottomWallB{{0, 0, 0}, {0, 0, 150}, {100, 0, 150}};

	// Top Ceiling
	const Triangle topWallA{{0, 100, 0}, {100, 100, 0}, {0, 100, 150}};
	const Triangle topWallB{{100, 100, 150}, {100, 100, 0}, {0, 100, 150}};

	const Sphere leftSphere{16.5, glm::vec3 {27, 16.5, 47}};
	const Sphere rightSphere{16.5, glm::vec3 {73, 16.5, 78}};

	const glm::vec3 light{50, 70, 81.6};

	// Materials
	const Diffuse white{{.75, .75, .75}};
	const Diffuse red{{.75, .25, .25}};
	const Diffuse blue{{.25, .25, .75}};

	const Glass glass{{.9, .1, .9}};
	const Mirror mirror{{.9, .9, .1}};

	// Objects
	// Note: this is a rather convoluted way of initialising a vector of unique_ptr ;)
	const std::vector<std::unique_ptr<Object>> objects = [] (){
		std::vector<std::unique_ptr<Object>> ret;
		ret.push_back(makeObject(backWallA, white));
		ret.push_back(makeObject(backWallB, white));
		ret.push_back(makeObject(topWallA, white));
		ret.push_back(makeObject(topWallB, white));
		ret.push_back(makeObject(bottomWallA, white));
		ret.push_back(makeObject(bottomWallB, white));
		ret.push_back(makeObject(rightWallA, blue));
		ret.push_back(makeObject(rightWallB, blue));
		ret.push_back(makeObject(leftWallA, red));
		ret.push_back(makeObject(leftWallB, red));

		ret.push_back(makeObject(leftSphere, mirror));
		ret.push_back(makeObject(rightSphere, glass));

		return ret;
	}();
}

/*************************************************************/

//thread_local std::default_random_engine generator;
//thread_local std::uniform_real_distribution<float> distribution(0.0,1.0);

/*float random_u()
{
	return distribution(generator);
}*/

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
        if (d < t && d >= 0.0f)
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
#define SQR_PRECISION 0.00390625f
#define AMBIANTE 0.03125f
#define BRILLANCE 16

float mini = 1000, maxi = -1000;

glm::vec3 radiance (const Ray & r)
{
    float d;
    const Object* obj = intersect(r, d);
    if(obj == nullptr)
        return glm::vec3(0,0,0);

    glm::vec3 pos = r.origin + r.direction*d;
    Ray rl = {scene::light, normalize(pos-scene::light)};

    float d2;
    const Object* obj2 = intersect(rl, d2);

    float d3 = glm::distance2(pos, rl.origin+rl.direction*d2);

    const float div = 2+AMBIANTE;
    glm::vec3 c = obj->albedo();
    if(obj2 == nullptr || d3 > SQR_PRECISION)
        return (c*AMBIANTE)/div;

    glm::vec3 n = obj->getNormal(pos);
    glm::vec3 vl = -rl.direction;
    float diffuse = fabsf(glm::dot(vl, n));
    //glm::dot((glm::abs(glm::dot(n,vl)*2.f*n) - vl)
    float speculaire = fabsf(pow(glm::dot(reflect(vl,n),-r.direction), BRILLANCE));


    if(diffuse >= 1.0)
        std::cout << diffuse << std::endl;
    c *= (diffuse+AMBIANTE+speculaire)/div;


    if(mini > (diffuse+AMBIANTE+speculaire))
        mini = (diffuse+AMBIANTE+speculaire);
    else if (maxi < (diffuse+AMBIANTE+speculaire))
        maxi = (diffuse+AMBIANTE+speculaire);
    return c;
}

/*************************************************************/
/*************************************************************/
/*************************************************************/

std::vector<glm::vec3> convolution(const std::vector<glm::vec3>& colors, int h, int w)
{
    std::vector<glm::vec3> colors2(colors);
    for(int y = 0;  y < h;  y++)
    {
        for(int x = 0;  x < w;  x++)
        {
            int i = y * w + x;
            float div = 8;
            colors2[i] = colors[i]*div;
            if(y > 0)
            {
                if(x > 0)   {
                    colors2[i] += colors[(y-1)*w + (x-1)];
                    div++;
                }
                colors2[i] += colors[(y-1)*w + x];
                div++;
                if(x < w-1) {
                    colors2[i] += colors[(y-1)*w + (x+1)];
                }
            }
            if(x > 0)   {
                colors2[i] += colors[y*w + (x-1)];
                div++;
            }
            if(x < w-1) {
                colors2[i] += colors[y*w + (x+1)];
                div++;
            }
            if(y < h-1)
            {
                if(x > 0)   {
                    colors2[i] += colors[(y+1)*w + (x-1)];
                    div++;
                }
                colors2[i] += colors[(y+1)*w + x];
                div++;
                if(x < w-1) {
                    colors2[i] += colors[(y+1)*w + (x+1)];
                    div++;
                }
            }
            colors2[i] /= div;
        }
    }
    return colors2;
}

/*************************************************************/

int main (int, char **)
{
	int w = 768, h = 768;
    std::vector<glm::vec3> colors(w * h, glm::vec3{0.f, 0.f, 0.f});

	Ray cam {{50, 52, 295.6}, glm::normalize(glm::vec3{0, -0.042612, -1})};	// cam pos, dir
	float near = 1.f;
	float far = 10000.f;

	glm::mat4 camera =
		glm::scale(glm::mat4(1.f), glm::vec3(float(w), float(h), 1.f))
		* glm::translate(glm::mat4(1.f), glm::vec3(0.5, 0.5, 0.f))
		* glm::perspective(float(54.5f * pi / 180.f), float(w) / float(h), near, far)
		* glm::lookAt(cam.origin, cam.origin + cam.direction, glm::vec3(0, 1, 0))
		;

	glm::mat4 screenToRay = glm::inverse(camera);

	for (int y = 0; y < h; y++)
    {
		std::cerr << "\rRendering: " << 100 * y / (h - 1) << "%";

		for (unsigned short x = 0; x < w; x++)
		{
			glm::vec4 p0 = screenToRay * glm::vec4{float(x), float(h - y), 0.f, 1.f};
			glm::vec4 p1 = screenToRay * glm::vec4{float(x), float(h - y), 1.f, 1.f};

			glm::vec3 pp0 = glm::vec3(p0 / p0.w);
			glm::vec3 pp1 = glm::vec3(p1 / p1.w);

			glm::vec3 d = glm::normalize(pp1 - pp0);

            glm::vec3 r = radiance (Ray{pp0, d});
            colors[y * w + x] += glm::clamp(r, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)) * 0.25f;
		}
    }

    //convolution
    for(int i = 0;  i < 4;  i++)
        colors = convolution(colors, h, w);

	{
		std::fstream f("image.ppm", std::fstream::out);
		f << "P3\n" << w << " " << h << std::endl << "255" << std::endl;

        for (auto c : colors)
			f << toInt(c.x) << " " << toInt(c.y) << " " << toInt(c.z) << " ";
	}
    std::cout << mini << "   " << maxi << std::endl;
}
