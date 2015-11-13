#include "mesh.h"

Mesh::Mesh(const glm::vec3 &center, const char* obj):
        center(center)
{
    box = Box();
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
            box.merge(p);
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

    //boundingSphere.C = 0.5*(minVal+maxVal);
    //boundingSphere.R = sqrt((maxVal-minVal).sqrNorm())*0.5;

    fclose(f);
}

glm::vec3 Mesh::getNormal(const glm::vec3& p, const glm::vec3& dir, int id) const    {
    glm::vec3 n = normals[normalIds[id]]/distance2(p,vertices[faces[id]]);
    n += normals[normalIds[id+1]]/distance2(p,vertices[faces[id+1]]);
    n += normals[normalIds[id+2]]/distance2(p,vertices[faces[id+2]]);
    return glm::normalize(n);
    //Triangle t{vertices[faces[id]],vertices[faces[id+1]],vertices[faces[id+2]]};
    //return t.getNormal(p, dir, id);
    (void) dir;
}
void Mesh::reposition(glm::vec3& pos, const glm::vec3& n, bool out) const
{
    if(!out)
        pos += n*0.0001f;
    else
        pos -= n*0.0001f;
}


float Mesh::intersect(const Ray& ray, int &id) const
{
    if(box.intersect(ray) == noIntersect)
        return noIntersect;

    float min = noIntersect;

    if(!haveBVH)
    {
        for(unsigned int i = 0;  i < faces.size();   i+=3)
        {
            TrianglePrim t{vertices[faces[i]],vertices[faces[i+1]],vertices[faces[i+2]]};
            int id2;
            float val = t.intersect(ray, id2);
            if(val < min)
            {
                min = val;
                id = i;
            }
        }
    }
    else
        min = bvh->intersect(ray, this->faces, this->vertices, id);

    return min;
}


/**********************************************************************/

void Mesh::translate(const glm::vec3& trans)
{
    center += trans;
    for(glm::vec3& p: vertices)
        p += trans;
    box.min += trans;
    box.max += trans;
}

void Mesh::scale(float s)
{
    for(glm::vec3& p: vertices) {
        p -= center;
        p *= s;
        p += center;
    }
    box.min -= center;
    box.min *= s;
    box.min += center;

    box.max -= center;
    box.max *= s;
    box.max += center;
}

void Mesh::scale(const glm::vec3& s)
{
    for(glm::vec3& p: vertices) {
        p -= center;
        p *= s;
        p += center;
    }

    for(glm::vec3& n: normals)  {
        n *= s;
        n = glm::normalize(n);
    }
    box.min -= center;
    box.min *= s;
    box.min += center;

    box.max -= center;
    box.max *= s;
    box.max += center;
}

void rotate2(glm::vec3& p, float angle, const glm::vec3& axe)
{
    float x = p.x,  y = p.y;

    p.x =   axe.x*(axe.x*x + axe.y*y + axe.z*p.z) * (1-cos(angle))+
            p.x*cos(angle)+
            (-axe.z*y + axe.y*p.z) * sin(angle);
    p.y =   axe.y*(axe.x*x + axe.y*y + axe.z*p.z) * (1-cos(angle))+
            p.y*cos(angle)+
            (axe.z*x - axe.x*p.z) * sin(angle);
    p.z =   axe.z*(axe.x*x + axe.y*y + axe.z*p.z) * (1-cos(angle))+
            p.z*cos(angle)+
            (-axe.y*x + axe.x*y) * sin(angle);
}

void Mesh::rotate(float angle, const glm::vec3& axe)
{
    box = Box();

    for(glm::vec3& p: vertices) {
        p -= center;
        rotate2(p, angle, axe);
        p += center;
        box.merge(p);
    }
    for(glm::vec3& n: normals)  {
        rotate2(n, angle, axe);
        n = glm::normalize(n);
    }
}


void Mesh::updateBVH(int nbTriMax)
{
    bvh = make_bvh_nb(faces, vertices, nbTriMax);
    haveBVH = true;
}
