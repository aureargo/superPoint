#ifndef MATERIAL_H
#define MATERIAL_H



#define NOIR glm::vec3(0.f, 0.f, 0.f)

bool aLaLumiere(const glm::vec3& p, const glm::vec3& l);
glm::vec3 radiance (const Ray & r, int radMax);
bool refract(glm::vec3 i, glm::vec3 n, float ior, glm::vec3 &wo);
glm::vec3 reflect(const glm::vec3& i, const glm::vec3& n);
glm::vec3 sample_cos(const float u, const float v, const glm::vec3& n);
float random_u();
glm::vec3 sample_sphere(float r, float u, float v, float &pdf, const glm::vec3& normal);


inline float speculaire(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, float brillance = 20)
{
    const glm::vec3 R = reflect(glm::normalize(l-p),n);
    return std::pow(fabsf(glm::dot(R,-cam.direction)), brillance);

}

inline glm::vec3 monteCarlo(const glm::vec3& l, const glm::vec3& p)
{
    return l+16.f*sample_cos(random_u(),random_u(),glm::normalize(l-p));
}

struct Diffuse  {
    const glm::vec3 color;

    glm::vec3 direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const
    {
        if(!aLaLumiere(p,monteCarlo(l,p)))
            return NOIR;
        float diffuse = fabsf(glm::dot(n, glm::normalize(monteCarlo(l,p)-p))) / pi;
        float specular = speculaire(cam, p, n, l);
        return (color * (diffuse+specular));
    }

    glm::vec3 indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const
    {
        /*glm::vec3 newDir = reflect(-cam.direction,n);
        glm::vec3 c = radiance(Ray{p, newDir}, radMax);
        return c*color;*/

        //glm::vec3 w = reflect(-cam.direction, n);
        glm::vec3 c(0.f,0.f,0.f);

        float angle = glm::dot(n, cam.direction);   // normalement glm::dot(n, -cam.direction) mais on inverse tout après
        glm::vec3 n2 = (angle < 0    ?   n   :   -n);
        glm::vec3 w(sample_cos(random_u(),random_u(), n2));

        c += (radiance(Ray{p,w}, radMax-1)*fabsf(glm::dot(n2,w)));

        return color*c;
        //return NOIR;
        (void) l;
    }


    glm::vec3 projection(const Ray& cam, const glm::vec3& n) const
    {
        float angle = glm::dot(n, cam.direction);   // normalement glm::dot(n, -cam.direction) mais on inverse tout après
        glm::vec3 n2 = (angle < 0    ?   n   :   -n);
        return glm::vec3(sample_cos(random_u(),random_u(), n2));
    }

    Ray projection(const Ray& cam, const glm::vec3& p, const glm::vec3& n) const
    {
        return Ray{p, projection(cam,n)};
    }
};

/*************************************************************************************/

struct Glass    {
    const glm::vec3 color;
    const float refractProp;

    glm::vec3 direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const
    {
        /*if(!aLaLumiere(p,l))
            return NOIR;
        float diffuse = fabsf(glm::dot(n, glm::normalize(l-p))) / pi;
        return (color * diffuse);*/

        if(aLaLumiere(p,monteCarlo(l,p)))
            return color*speculaire(cam, p, n, l);
        else
            //return color;
            return NOIR;
    }


    glm::vec3 projection(const Ray& cam, const glm::vec3& n) const
    {
        glm::vec3 wo;
        if(!refract(-cam.direction, n, refractProp, wo))
            wo = reflect(-cam.direction,n);
        return wo;
    }

    Ray projection(const Ray& cam, const glm::vec3& p, const glm::vec3& n) const
    {
        return Ray{p, projection(cam,n)};
    }

    glm::vec3 indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const
    {
        glm::vec3 c = radiance(projection(cam, p, n), radMax);
        return c*color;//glm::vec3(c.r*color.r, c.g*color.g, c.b*color.b);
        //return c;
        (void) l;
    }

};

/*************************************************************************************/

struct Mirror   {
    const glm::vec3 color;

    glm::vec3 direct(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l) const
    {
        /*if(!aLaLumiere(p,l))
            return NOIR;
        float diffuse = fabsf(glm::dot(n, glm::normalize(l-p))) / pi;
        return (color * diffuse);*/
        if(aLaLumiere(p,l))
            return color*speculaire(cam, p, n, l);
        else
            //return color;
            return NOIR;
        (void)cam;(void)p;(void)n;(void)l;
    }


    glm::vec3 projection(const Ray& cam, const glm::vec3& n) const
    {
        return reflect(-cam.direction,n);
    }

    Ray projection(const Ray& cam, const glm::vec3& p, const glm::vec3& n) const
    {
        return Ray{p, projection(cam,n)};
    }

    glm::vec3 indirect(const Ray& cam, const glm::vec3& p, const glm::vec3& n, const glm::vec3& l, int radMax) const
    {
        glm::vec3 newDir = reflect(-cam.direction,n);
        glm::vec3 c = radiance(Ray{p, newDir}, radMax);
        return c*color;//glm::vec3(c.r*color.r, c.g*color.g, c.b*color.b);
        (void) l;
    }

};

/*************************************************************/

template<typename T>
glm::vec3 albedo(const T &t)
{
    return t.color;
}




#endif // MATERIAL_H
