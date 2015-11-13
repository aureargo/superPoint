#include "bvh_feuille.h"

BVH_feuille::BVH_feuille()
{
}

BVH_feuille::BVH_feuille(const std::vector<int>& faces, const std::vector<glm::vec3>& points)
{
    int taille = faces.size()/3;
    idTriangles.reserve(taille);
    for(int i = 0;  i < taille; i++)        {
        idTriangles.push_back(i);
        box.merge(points[faces[i*3]]);
        box.merge(points[faces[i*3+1]]);
        box.merge(points[faces[i*3+2]]);
    }
}

BVH_feuille::BVH_feuille(const std::list<TriangleId>& triangles)
{
    idTriangles.reserve(triangles.size());
    for(const TriangleId& t: triangles){
        idTriangles.push_back(t.id);
        box.merge(t.box);
    }
}

float BVH_feuille::intersect(const Ray& ray, const std::vector<int>& faces, const std::vector<glm::vec3>& points, int &id) const
{
    float min = noIntersect;

    for(int id2: idTriangles)
    {
        int i = id2*3;
        Triangle t{points[faces[i]],points[faces[i+1]],points[faces[i+2]]};
        float val = t.intersect(ray);
        if(val < min)
        {
            min = val;
            id = i;
        }
    }
    return min;
}

Box BVH_feuille::calculBox() const
{
    return box;
}
