#include "bvh_noeud.h"

BVH_noeud::BVH_noeud()
{
}

Box BVH_noeud::calculBox() const
{
    Box box;
    for(const std::unique_ptr<BVH>& child : children)
        box.merge(child->box);
    return box;
}
