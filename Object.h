#ifndef OBJECT_H
#define OBJECT_H

#include "material.h"

/*************************************************************/

struct Object
{
    virtual float intersect(const Ray &r) const = 0;
    virtual glm::vec3 albedo() const = 0;
    virtual glm::vec3 getNormal(const glm::vec3& pos) const = 0;
    virtual glm::vec3 direct(const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const = 0;
    virtual glm::vec3 indirect(const Ray& c, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax = 10) const = 0;
    virtual void reposition(glm::vec3& pos, const glm::vec3& n, const glm::vec3& dir, bool out) const = 0;
    //cos(theta)/pi

};

template<typename P, typename M>
struct ObjectTpl final : Object
{
    ObjectTpl(const P &_p, const M &_m)
        :primitive(_p), material(_m)
    {}

    float intersect(const Ray &ray) const	{
        return ::intersect(ray, primitive);
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


    glm::vec3 direct(const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const
    {
        return material.direct(p, n, l);
    }
    glm::vec3 indirect(const Ray& c, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const
    {
        return material.indirect(c, p, n, l, radMax);
    }


    const P &primitive;
    const M &material;
};


template<typename P, typename M>
std::unique_ptr<Object> makeObject(const P&p, const M&m){
    return std::unique_ptr<Object>(new ObjectTpl<P, M>{p, m});
}

#endif // OBJECT_H
