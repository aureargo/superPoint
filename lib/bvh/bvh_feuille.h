#ifndef BVH_FEUILLE_H
#define BVH_FEUILLE_H

#include "bvh.h"


class BVH_feuille:  public BVH
{
public:
    BVH_feuille();
    BVH_feuille(const std::vector<int>& faces, const std::vector<glm::vec3>& points);
    BVH_feuille(const std::list<TriangleId>& triangles);


    float intersect(const Ray &ray, const std::vector<int>& faces, const std::vector<glm::vec3>& points, int& id) const;

private:
    std::vector<int> idTriangles;

    Box calculBox() const;
};

#endif // BVH_FEUILLE_H
