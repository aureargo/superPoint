#ifndef OBJECT_H
#define OBJECT_H

#include "global.h"
#include "lumiere/lumiere.h"




/*************************************************************/


/**
 * @brief The Object struct
 * déplacé pour cause d'inclusion mutuelle des fichiers.
 */
struct Object
{
    virtual float intersect(const Ray &r, int& id) const = 0;
    virtual glm::vec3 albedo() const = 0;
    virtual glm::vec3 getNormal(const glm::vec3& pos, const glm::vec3& dir, int id) const = 0;
    virtual glm::vec3 direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const Lumiere& l) const = 0;
    virtual glm::vec3 indirect(const Ray& c, const glm::vec3& p, const glm::vec3& n, const Lumiere& l, int radMax = 10) const = 0;
    virtual void reposition(glm::vec3& pos, const glm::vec3& n, bool out) const = 0;
    virtual Ray projection(const Ray& c, const glm::vec3& p, const glm::vec3& n) const = 0;
    virtual glm::vec3 projection(const Ray& c, const glm::vec3& n) const = 0;

    //cos(theta)/pi

};

/*************************************************************/






#endif // OBJECT_H
