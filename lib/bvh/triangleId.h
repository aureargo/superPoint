#ifndef TRI_H
#define TRI_H

#include "lib/box.h"
#include "lib/triangle.h"


class TriangleId: public Triangle
{
public:
    int id;
    Box box;

    TriangleId(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, int id):
            Triangle(p0,p1,p2), id(id)
    {
        box.merge(p0);
        box.merge(p1);
        box.merge(p2);
    }
};



struct cmpVec3X {
    bool operator() (TriangleId const& lhs, TriangleId const& rhs) const
    {
        return (lhs.box.max.x < rhs.box.max.x);
    }
};

struct cmpVec3Y {
    bool operator() (TriangleId const& lhs, TriangleId const& rhs) const
    {
        return (lhs.box.max.y < rhs.box.max.y);
    }
};
struct cmpVec3Z {
    bool operator() (TriangleId const& lhs, TriangleId const& rhs) const
    {
        return (lhs.box.max.z < rhs.box.max.z);
    }
};

#endif // TRI_H
