#ifndef BVH_NB_NOEUD_H
#define BVH_NB_NOEUD_H

#include "bvh_noeud.h"
#include "bvh_feuille.h"

class BVH_nb_noeud: public BVH_noeud
{
public:
    BVH_nb_noeud();

    BVH_nb_noeud(const std::vector<int>& faces, const std::vector<glm::vec3>& points, int nbTriMax = 100);
    BVH_nb_noeud(std::list<TriangleId>& triangles, int nbTriMax = 100);

    float intersect(const Ray &ray, const std::vector<int>& faces, const std::vector<glm::vec3>& points, int& id) const;

private:
    void decoupe(std::list<TriangleId>& triangles);

    int nbTriMax;
};

BVH* make_bvh_nb(const std::vector<int>& faces, const std::vector<glm::vec3>& points, int nbTriMax);
BVH* make_bvh_nb(std::list<TriangleId>& triangles, int nbTriMax);

#endif // BVH_NB_NOEUD_H
