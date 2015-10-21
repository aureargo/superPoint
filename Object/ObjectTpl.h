#ifndef OBJECTTPL_H
#define OBJECTTPL_H

#include "Object.h"
#include "material/material.h"

template<typename P, typename M>
struct ObjectTpl final : Object
{
    ObjectTpl(const P &_p, const M &_m)
        :primitive(_p), material(_m)
    {}

    float intersect(const Ray &ray) const	{
        return primitive.intersect(ray);
    }

    glm::vec3 albedo() const	{
        return ::albedo(material);
    }

    glm::vec3 getNormal(const glm::vec3& p) const    {
        return primitive.getNormal(p);
    }

    void reposition(glm::vec3& pos, const glm::vec3& n, const glm::vec3& dir, bool out) const
    {
        return primitive.reposition(pos, n, dir, out);
    }


    glm::vec3 direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const
    {
        return material.direct(cam, p, n, l);
    }
    glm::vec3 indirect(const Ray& c, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const
    {
        glm::vec3 p2 = p;
        reposition(p2,n,c.direction,false);
        return material.indirect(c, p2, n, l, radMax);
    }

    Ray projection(const Ray& c, const glm::vec3& p, const glm::vec3& n) const
    {
        return Ray(p, material.projection(c, n));    //material.projection(c, p, n);
    }

    glm::vec3 projection(const Ray& c, const glm::vec3& n) const
    {
        return material.projection(c, n);
    }

    const P &primitive;
    const M &material;
};

/*************************************************************/


template<typename P, typename M>
std::unique_ptr<Object> makeObject(const P&p, const M&m){
    return std::unique_ptr<Object>(new ObjectTpl<P, M>{p, m});
}


#endif // OBJECTTPL_H
