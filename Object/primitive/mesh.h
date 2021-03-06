#ifndef MESH_H
#define MESH_H

#include "primitive.h"
#include "lib/box.h"
#include "lib/bvh/bvh_nb_noeud.h"

class Mesh: public Primitive
{
public:
    std::vector<glm::vec3> vertices, normals;
    std::vector<int> faces, normalIds;
    BVH* bvh;

    Box box;
    glm::vec3 center;
    bool haveBVH = false;

    Mesh(const glm::vec3 &center, const char* obj);

    glm::vec3 getNormal(const glm::vec3& p, const glm::vec3& dir, int id) const;
    void reposition(glm::vec3& pos, const glm::vec3& n, bool out) const;
    float intersect(const Ray &ray, int& id) const;


    void translate(const glm::vec3& trans);
    void scale(float s);
    void scale(const glm::vec3& s);
    void rotate(float angle, const glm::vec3& axe);
    void updateBVH(int nbTriMax = 100);
};

#endif // MESH_H
