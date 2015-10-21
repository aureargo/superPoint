#ifndef INTERSECT_H
#define INTERSECT_H

#include "global.h"
#include "Object/primitive/primitive.h"
#include "ray.h"



float intersect(const Ray & ray, const Sphere &sphere);
float intersect(const Ray & ray, const Triangle &triangle);
float intersect(const Ray & ray, const Box &box);
float intersect(const Ray & ray, const Mesh &mesh);






#endif // INTERSECT_H
