#ifndef BVH_NOEUD_H
#define BVH_NOEUD_H

#include "bvh.h"

class BVH_noeud: public BVH
{
public:
    BVH_noeud();

protected:
    Box calculBox() const;
    std::vector<std::unique_ptr<BVH> > children;


private:
    virtual void decoupe(std::list<TriangleId>& triangles) = 0;

};

#endif // BVH_NOEUD_H
