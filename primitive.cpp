#include "primitive.h"


struct Ray
{
    glm::vec3 origin, direction;
};

struct Sphere
{
    const float radius;
    const glm::vec3 center;

    glm::vec3 getNormal(const glm::vec3& p) const    {
        return glm::normalize(p-center);
    }
    void reposition(glm::vec3& pos, const glm::vec3& n, const glm::vec3& dir, bool out) const
    {
        float r2 = radius;
        r2 += (out  ?   0.0001f   :   -0.0001f);

        pos = center + n*r2;
        (void) n;   (void) dir;
    }
};

struct Triangle
{
    const glm::vec3 v0, v1, v2;

    glm::vec3 getNormal(const glm::vec3& p) const    {
        return glm::normalize(glm::cross(v1-v0, v2-v0));
        (void) p;
    }
    void reposition(glm::vec3& pos, const glm::vec3& n, const glm::vec3& dir, bool out) const
    {
        if(distance2(n,dir) > 2)
            out = !out;

        (void) pos;
    }
};



struct Box
{
    glm::vec3 min, max;

    Box():
            min(glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX)),    max(glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN))
    {
    }

    Box(const glm::vec3& min, const glm::vec3& max):
            min(min),   max(max)
    {
    }
};

struct Mesh
{
    std::vector<glm::vec3> vertices, normals;
    std::vector<int> faces, normalIds;
    Box box;

    Mesh(const glm::vec3 &center, const char* obj) {
        glm::vec3 minVal(FLT_MAX, FLT_MAX, FLT_MAX), maxVal(FLT_MIN, FLT_MIN, FLT_MIN);
        FILE* f = fopen(obj, "r");
        while (!feof(f)) {
            char line[255];
            fgets(line, 255, f);
            if (line[0]=='v' && line[1]==' ') {
                glm::vec3 vec;
                sscanf(line, "v %f %f %f\n", &vec[0], &vec[2], &vec[1]);
                vec[2] = -vec[2];
                glm::vec3 p = vec*50.f + center;
                vertices.push_back(p);
                maxVal[0] = std::max(maxVal[0], p[0]);
                maxVal[1] = std::max(maxVal[1], p[1]);
                maxVal[2] = std::max(maxVal[2], p[2]);
                minVal[0] = std::min(minVal[0], p[0]);
                minVal[1] = std::min(minVal[1], p[1]);
                minVal[2] = std::min(minVal[2], p[2]);
            }
            if (line[0]=='v' && line[1]=='n') {
                glm::vec3 vec;
                sscanf(line, "vn %f %f %f\n", &vec[0], &vec[2], &vec[1]);
                vec[2] = -vec[2];
                normals.push_back(vec);
            }
            if (line[0]=='f') {
                int i0, i1, i2;
                int j0,j1,j2;
                int k0,k1,k2;
                sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u\n", &i0, &j0, &k0, &i1, &j1, &k1, &i2, &j2, &k2 );
                faces.push_back(i0-1);
                faces.push_back(i1-1);
                faces.push_back(i2-1);
                normalIds.push_back(k0-1);
                normalIds.push_back(k1-1);
                normalIds.push_back(k2-1);
            }

        }

        box = Box(minVal, maxVal);
        //boundingSphere.C = 0.5*(minVal+maxVal);
        //boundingSphere.R = sqrt((maxVal-minVal).sqrNorm())*0.5;

        fclose(f);
    }

    glm::vec3 getNormal(const glm::vec3& p) const    {
        /*float min = noIntersect;
        for(unsigned int i = 0;  i < mesh.faces.size();   i+=3)
        {
            Triangle t{mesh.vertices[mesh.faces[i]],mesh.vertices[mesh.faces[i+1]],mesh.vertices[mesh.faces[i+2]]};
            float val = intersect(ray, t);
            if(val < min)
                min = val;
        }*/
        return glm::vec3(0,0,1);
    }
    void reposition(glm::vec3& pos, const glm::vec3& n, const glm::vec3& dir, bool out) const
    {
        return;
        (void) n;   (void) dir;
    }
};


/*********************************************************************************************/

