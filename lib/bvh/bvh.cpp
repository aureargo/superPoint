#include "bvh.h"

BVH::BVH()
{
}

BVH::BVH(const BVH& copy):
    nbTriMax(copy.nbTriMax),    nbChildren(copy.nbChildren), idTriangles(copy.idTriangles), box(copy.box)
{
    if(nbChildren > 0)
    {
        children = new BVH*[nbChildren];
        for(int i = 0;  i < nbChildren; i++)
            children[i] = new BVH(*copy.children[i]);
    }
}

BVH::BVH(BVH&& copy):
    nbTriMax(copy.nbTriMax),    nbChildren(copy.nbChildren), idTriangles(copy.idTriangles), box(copy.box)
{
    children = copy.children;
    copy.children = nullptr;
    copy.nbChildren = 0;
}

void BVH::operator= (const BVH& copy)
{
    nbTriMax = copy.nbTriMax;
    nbChildren = copy.nbChildren;
    idTriangles = copy.idTriangles;
    box = copy.box;
    if(nbChildren > 0)
    {
        children = new BVH*[nbChildren];
        for(int i = 0;  i < nbChildren; i++)
            children[i] = new BVH(*copy.children[i]);
    }
}
void BVH::operator= (BVH&& copy)
{
    nbTriMax = copy.nbTriMax;
    nbChildren = copy.nbChildren;
    idTriangles = copy.idTriangles;
    box = copy.box;

    children = copy.children;
    copy.children = nullptr;
    copy.nbChildren = 0;
}

BVH::~BVH()
{
    if(nbChildren > 0)
    {
        for(int i = 0;  i < nbChildren; i++)
            delete children[i];
        delete[] children;
    }
    //nbChildren = 0;
}

BVH::BVH(const std::vector<int>& faces, const std::vector<glm::vec3>& points, int nbTriMax):
        nbTriMax(nbTriMax)
{
    int taille = faces.size()/3;
    if(taille < nbTriMax)
    {
        idTriangles.reserve(taille);
        for(int i = 0;  i < taille; i++)        {
            idTriangles.push_back(i);
            box.merge(points[faces[i*3]]);
            box.merge(points[faces[i*3+1]]);
            box.merge(points[faces[i*3+2]]);
        }
    }
    else
    {
        std::list<TriangleId> triangles;
        //triangles.reserve(taille);

        for(int i = 0;  i < taille; i++)   {
            const TriangleId t(points[faces[i*3]], points[faces[i*3+1]], points[faces[i*3+2]], i);
            triangles.push_back(t);
            box.merge(t.box);
        }

        decoupe(triangles);
    }
}

BVH::BVH(std::list<TriangleId>& triangles, int nbTriMax):
        nbTriMax(nbTriMax)
{
    int taille = triangles.size();
    if(taille < nbTriMax)
    {
        idTriangles.reserve(taille);
        for(const TriangleId& t: triangles){
            idTriangles.push_back(t.id);
            box.merge(t.box);
        }
    }
    else
    {
        for(const TriangleId& t: triangles)
            box.merge(t.box);
        decoupe(triangles);
    }
}

void BVH::decoupe(std::list<TriangleId>& triangles)
{
    nbChildren = 2;
    int moitie = triangles.size()/2;

    int axe = box.axeMax();
    switch(axe)
    {
        case 0 :    triangles.sort(cmpVec3X()); break;
        case 1 :    triangles.sort(cmpVec3Y()); break;
        default:    triangles.sort(cmpVec3Z()); break;
    }


    std::list<TriangleId> triangles2;
    std::list<TriangleId>::iterator it = triangles.begin();
    std::advance(it, moitie);
    triangles2.splice(triangles2.begin(), triangles, it, triangles.end());

    children = new BVH*[nbChildren];
    children[0] = new BVH(triangles, nbTriMax);
    children[1] = new BVH(triangles2, nbTriMax);
}


float BVH::intersect(const Ray& ray, const std::vector<int>& faces, const std::vector<glm::vec3>& points, int &id) const
{
    //if(box.intersect(ray) == noIntersect)
    //    return noIntersect;

    float min = noIntersect;

    if(nbChildren == 0)
    {
        for(int id2: idTriangles)
        {
            int i = id2*3;
            Triangle t{points[faces[i]],points[faces[i+1]],points[faces[i+2]]};
            float val = t.intersect(ray);
            if(val < min)
            {
                min = val;
                id = i;
            }
        }
    }
    else
    {
        float distanceMin1, distanceMax1,
                distanceMin2, distanceMax2;
        bool b1 = children[0]->box.intersect(ray, distanceMin1, distanceMax1);
        bool b2 = children[1]->box.intersect(ray, distanceMin2, distanceMax2);

        if(!b1 && !b2)
            return noIntersect;
        else if(b1)
        {
            if(!b2)
                return children[0]->intersect(ray, faces, points, id);
            else
            {
                float min2;
                int id2;
                if(distanceMin1 <= distanceMin2)
                {
                    min = children[0]->intersect(ray, faces, points, id);
                    if(min <= distanceMin2)
                        return min;
                    else
                    {
                        min2 = children[1]->intersect(ray, faces, points, id2);
                        if(min > min2){
                            id = id2;
                            return min2;
                        }
                    }
                }
                else
                {
                    min = children[1]->intersect(ray, faces, points, id);
                    if(min <= distanceMin1)
                        return min;
                    else
                    {
                        min2 = children[0]->intersect(ray, faces, points, id2);
                        if(min > min2){
                            id = id2;
                            return min2;
                        }
                    }
                }
            }
        }
        else    //if(b2 && !b1)
            return children[1]->intersect(ray, faces, points, id);
    }
    return min;
}
