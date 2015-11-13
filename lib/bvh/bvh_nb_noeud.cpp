#include "bvh_nb_noeud.h"


BVH* make_bvh_nb(const std::vector<int>& faces, const std::vector<glm::vec3>& points, int nbTriMax)
{
    if((int)faces.size()/3 <= nbTriMax)
        return new BVH_feuille(faces, points);
    else
        return new BVH_nb_noeud(faces, points, nbTriMax);
}

BVH* make_bvh_nb(std::list<TriangleId>& triangles, int nbTriMax)
{
    if((int)triangles.size() <= nbTriMax)
        return new BVH_feuille(triangles);
    else
        return new BVH_nb_noeud(triangles, nbTriMax);
}


/************************************************************************************************************/


BVH_nb_noeud::BVH_nb_noeud()
{
}


BVH_nb_noeud::BVH_nb_noeud(const std::vector<int>& faces, const std::vector<glm::vec3>& points, int nbTriMax):
        nbTriMax(nbTriMax)
{
    int taille = faces.size()/3;
    std::list<TriangleId> triangles;
    //triangles.reserve(taille);

    for(int i = 0;  i < taille; i++)   {
        TriangleId t(points[faces[i*3]], points[faces[i*3+1]], points[faces[i*3+2]], i);
        triangles.push_back(t);
    }

    decoupe(triangles);
    box = calculBox();
}

BVH_nb_noeud::BVH_nb_noeud(std::list<TriangleId>& triangles, int nbTriMax):
        nbTriMax(nbTriMax)
{
    decoupe(triangles);
    box = calculBox();
}


void BVH_nb_noeud::decoupe(std::list<TriangleId>& triangles)
{
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

    children.reserve(2);
    children.push_back(std::unique_ptr<BVH>(make_bvh_nb(triangles, nbTriMax)));
    children.push_back(std::unique_ptr<BVH>(make_bvh_nb(triangles2, nbTriMax)));
}



float BVH_nb_noeud::intersect(const Ray& ray, const std::vector<int>& faces, const std::vector<glm::vec3>& points, int &id) const
{
    float min = noIntersect;

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
                    return min;
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
                    return min;
                }
            }
        }
    }
    else    //if(b2 && !b1)
        return children[1]->intersect(ray, faces, points, id);
}
