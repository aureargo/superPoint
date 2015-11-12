#ifndef BVH_H
#define BVH_H

#include <vector>
#include <list>
#include "lib/box.h"
#include "triangleId.h"


class BVH
{
public:

    BVH();
    BVH(const BVH& copy);
    BVH(BVH&& copy);

    ~BVH();
    BVH(const std::vector<int>& faces, const std::vector<glm::vec3>& points, int nbTriMax = 100);
    BVH(std::list<TriangleId>& triangles, int nbTriMax = 100);

    void operator= (const BVH& copy);
    void operator= (BVH&& copy);


    float intersect(const Ray &ray, const std::vector<int>& faces, const std::vector<glm::vec3>& points, int& id) const;

private:
    void decoupe(std::list<TriangleId>& triangles);

    int nbTriMax;
    int nbChildren = 0;
    BVH** children = nullptr;
    std::vector<int> idTriangles;
    Box box;
};

#endif // BVH_H
