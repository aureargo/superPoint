#ifndef BVH_H
#define BVH_H

#include <vector>
#include <list>
#include "lib/box.h"
#include "triangleId.h"

class BVH
{
public:
    BVH()   {}

    virtual float intersect(const Ray &ray, const std::vector<int>& faces, const std::vector<glm::vec3>& points, int& id) const = 0;


    Box box;

protected:

private:
    virtual Box calculBox() const = 0;
};


#endif // BVH_H
