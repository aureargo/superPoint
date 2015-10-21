#ifndef MESH_H
#define MESH_H

#include "primitive.h"
#include "box.h"

class Mesh: public Primitive
{
public:
    std::vector<glm::vec3> vertices, normals;
    std::vector<int> faces, normalIds;
    Box box;

    Mesh(const glm::vec3 &center, const char* obj);

    glm::vec3 getNormal(const glm::vec3& p) const;
    void reposition(glm::vec3& pos, const glm::vec3& n, const glm::vec3& dir, bool out) const;
    float intersect(const Ray &ray) const;

};

#endif // MESH_H
