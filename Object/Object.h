#ifndef OBJECT_H
#define OBJECT_H

#include "global.h"




/*************************************************************/


/**
 * @brief The Object struct
 * déplacé pour cause d'inclusion mutuelle des fichiers.
 */
struct Object
{
    virtual float intersect(const Ray &r) const = 0;
    virtual glm::vec3 albedo() const = 0;
    virtual glm::vec3 getNormal(const glm::vec3& pos) const = 0;
    virtual glm::vec3 direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const = 0;
    virtual glm::vec3 indirect(const Ray& c, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax = 10) const = 0;
    virtual void reposition(glm::vec3& pos, const glm::vec3& n, const glm::vec3& dir, bool out) const = 0;
    virtual Ray projection(const Ray& c, const glm::vec3& p, const glm::vec3& n) const = 0;
    virtual glm::vec3 projection(const Ray& c, const glm::vec3& n) const = 0;

    //cos(theta)/pi

};

/*************************************************************/






#endif // OBJECT_H
